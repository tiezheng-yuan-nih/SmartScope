import os
import sys
import time
import multiprocessing
import logging
from pathlib import Path
from django.db import transaction
from django.utils import timezone
from django.conf import settings

logger = logging.getLogger(__name__)

from .grid.grid_status import GridStatus
from .grid.finders import find_targets
from .grid.grid_io import GridIO
from .grid.run_square import RunSquare
from .grid.run_hole import RunHole

from .interfaces.microscope_interface import MicroscopeInterface

from Smartscope.core.selectors import selector_wrapper
from Smartscope.core.models import ScreeningSession, SquareModel, AutoloaderGrid
from Smartscope.core.settings.worker import PROTOCOL_COMMANDS_FACTORY
from Smartscope.core.status import status
from Smartscope.core.protocols import get_or_set_protocol
from Smartscope.core.preprocessing_pipelines import load_preprocessing_pipeline
from Smartscope.core.db_manipulations import update, select_n_areas, queue_atlas, add_targets

from Smartscope.lib.image_manipulations import export_as_png


def run_grid(
        grid:AutoloaderGrid,
        session:ScreeningSession,
        processing_queue:multiprocessing.JoinableQueue,
        scope:MicroscopeInterface
    ):
    """Main logic for the SmartScope process
    Args:
        grid (AutoloaderGrid): AutoloadGrid object from Smartscope.server.models
        session (ScreeningSession): ScreeningSession object from Smartscope.server.models
    """
    logger.info(f'###Check status of grid, grid ID={grid.grid_id}.')
    session_id = session.pk
    microscope = session.microscope_id

    # Set the Websocket_update_decorator grid property
    update.grid = grid
    if grid.status == GridStatus.COMPLETED:
        logger.info(f'Grid {grid.name} already complete. grid ID={grid.grid_id}')
        return
    
    if grid.status == GridStatus.ABORTING:
        logger.info(f'Aborting {grid.name}')
        update(grid, status=GridStatus.COMPLETED)
        return

    logger.info(f'Starting {grid.name}, status={grid.status}') 
    grid = update(grid, refresh_from_db=True, last_update=None)
    if grid.status is GridStatus.NULL:
        grid = update(grid, status=GridStatus.STARTED, start_time=timezone.now())

    # add task into queue
    GridIO.create_grid_directories(grid.directory)
    logger.info(f"create and the enter into Grid directory={grid.directory}")
    os.chdir(grid.directory)
    processing_queue.put([os.chdir, [grid.directory], {}])
    params = grid.params_id

    # ADD the new protocol loader
    protocol = get_or_set_protocol(grid)
    resume_incomplete_processes(processing_queue, grid, session.microscope_id)
    preprocessing = load_preprocessing_pipeline(Path('preprocessing.json'))
    preprocessing.start(grid)
    is_stop_file(session_id)
    atlas = queue_atlas(grid)

    # scope
    scope.loadGrid(grid.position)
    is_stop_file(session_id)
    scope.setup(params.save_frames, framesName=f'{session.date}_{grid.name}')
    scope.reset_state()
    # grid_type = grid.holeType
    # grid_mesh = grid.meshMaterial

    # run acquisition
    if atlas.status == status.QUEUED or atlas.status == status.STARTED:
        atlas = update(atlas, status=status.STARTED)
        logger.info('Waiting on atlas file')
        runAcquisition(
            scope,
            protocol.atlas.acquisition,
            params,
            atlas
        )
        atlas = update(atlas,
            status=status.ACQUIRED,
            completion_time=timezone.now()
        )

    # find targets
    if atlas.status == status.ACQUIRED:
        logger.info('Atlas acquired')
        montage = RunSquare.get_file_and_process(
            raw=atlas.raw,
            name=atlas.name,
            directory=microscope.scope_path
        )
        export_as_png(montage.image, montage.png)
        targets, finder_method, classifier_method, _ = find_targets(
            montage,
            protocol.atlas.targets.finders
        )
        squares = add_targets(
            grid,
            atlas,
            targets,
            SquareModel,
            finder_method,
            classifier_method
        )
        atlas = update(atlas,
            status=status.PROCESSED,
            pixel_size=montage.pixel_size,
            shape_x=montage.shape_x,
            shape_y=montage.shape_y,
            stage_z=montage.stage_z
        )
    
    # 
    if atlas.status == status.PROCESSED:
        selector_wrapper(protocol.atlas.targets.selectors, atlas, n_groups=5)
        select_n_areas(atlas, grid.params_id.squares_num)
        atlas = update(atlas, status=status.COMPLETED)
    logger.info('Atlas analysis is complete')


    running = True
    is_done = False
    while running:
        is_stop_file(session_id)
        grid = update(grid, refresh_from_db=True, last_update=None)
        params = grid.params_id
        if grid.status == GridStatus.ABORTING:
            preprocessing.stop(grid)
            break
        else:
            square, hole = get_queue(grid)

        if hole is not None and (square is None or grid.collection_mode == 'screening'):
            is_done = False

            # process medium image
            hole = update(hole, status=status.STARTED)
            runAcquisition(
                scope,
                protocol.mediumMag.acquisition,
                params,
                hole
            )
            hole = update(hole,
                status=status.ACQUIRED,
                completion_time=timezone.now()
            )
            RunHole.process_hole_image(hole, grid, microscope)

            # process high image
            scope.focusDrift(
                params.target_defocus_min,
                params.target_defocus_max,
                params.step_defocus,
                params.drift_crit
            )
            scope.reset_image_shift_values()
            for hm in hole.targets.exclude(status__in=[status.ACQUIRED,status.COMPLETED]).order_by('hole_id__number'):
                hm = update(hm, status=status.STARTED)
                hm = runAcquisition(
                    scope,
                    protocol.highMag.acquisition,
                    params,
                    hm
                )
                hm = update(hm,
                    status=status.ACQUIRED,
                    completion_time=timezone.now(),
                    extra_fields=['is_x','is_y','offset','frames']
                )
                if hm.hole_id.bis_type != 'center':
                    update(hm.hole_id, status=status.ACQUIRED, completion_time=timezone.now())
            update(hole, status=status.COMPLETED)
            scope.reset_AFIS_image_shift(afis=params.afis)
            scope.refineZLP(params.zeroloss_delay)
            scope.collectHardwareDark(params.hardwaredark_delay)
        elif square is not None:
            is_done = False

            # process square
            if square.status == status.QUEUED or square.status == status.STARTED:
                square = update(square, status=status.STARTED)
                logger.info('Waiting on square file')
                runAcquisition(
                    scope,
                    protocol.square.acquisition,
                    params,
                    square
                )
                square = update(square, status=status.ACQUIRED, completion_time=timezone.now())
                RunSquare.process_square_image(square, grid, microscope)
        elif is_done:
            microscope_id = session.microscope_id.pk
            tmp_file = os.path.join(settings.TEMPDIR, f'.pause_{microscope_id}')
            if os.path.isfile(tmp_file):
                paused = os.path.join(settings.TEMPDIR, f'paused_{microscope_id}')
                open(paused, 'w').close()
                update(grid, status=GridStatus.PAUSED)
                logger.info('SerialEM is paused')
                while os.path.isfile(paused):
                    sys.stdout.flush()
                    time.sleep(3)
                next_file = os.path.join(settings.TEMPDIR, f'next_{microscope_id}')
                if os.path.isfile(next_file):
                    os.remove(next_file)
                    running = False
                else:
                    update(grid, status=GridStatus.STARTED)
            else:
                running = False
        else:
            logger.debug(f'Waiting for incomplete processes, {processing_queue.get()}, queue size={processing_queue.qsize()}')
            processing_queue.join()
            logger.debug('All processes complete')
            is_done = True
        logger.debug(f'Running: {running}')
    else:
        update(grid, status=GridStatus.COMPLETED)
        logger.info('Grid finished')
        return 'finished'



def get_queue(grid):
    square = grid.squaremodel_set.\
        filter(selected=True, status__in=[status.QUEUED, status.STARTED]).\
        order_by('number').first()
    hole = grid.holemodel_set.filter(selected=True, square_id__status=status.COMPLETED).\
        exclude(status=status.COMPLETED).\
        order_by('square_id__completion_time', 'number').first()
    return square, hole#[h for h in holes if not h.bisgroup_acquired]



def resume_incomplete_processes(queue, grid, microscope_id):
    """Query database for models with incomplete processes
     and adds them to the processing queue

    Args:
        queue (multiprocessing.JoinableQueue): multiprocessing queue of objects
          for processing by    the processing_worker
        grid (AutoloaderGrid): AutoloadGrid object from Smartscope.server.models
        session (ScreeningSession): ScreeningSession object from Smartscope.server.models
    """
    squares = grid.squaremodel_set.filter(selected=1).exclude(
        status__in=[status.QUEUED, status.STARTED, status.COMPLETED]).order_by('number')
    holes = grid.holemodel_set.filter(selected=1).exclude(status__in=[status.QUEUED, \
        status.STARTED, status.PROCESSED, status.COMPLETED]).\
        order_by('square_id__number', 'number')
    for square in squares:
        logger.info(f'Square {square} was not fully processed')
        renew_queue = [RunSquare.process_square_image, [square, grid, microscope_id], {}]
        transaction.on_commit(lambda: queue.put(renew_queue))



def is_stop_file(sessionid: str) -> bool:
    stop_file = os.path.join(settings.TEMPDIR, f'{sessionid}.stop')
    if os.path.isfile(stop_file):
        logger.debug(f'Stop file {stop_file} found.')
        os.remove(stop_file)
        raise KeyboardInterrupt()


def runAcquisition(
        scope,
        methods,
        params,
        instance
    ):
    for method in methods:
        output = PROTOCOL_COMMANDS_FACTORY[method](scope,params,instance)
    return output



    




# def print_queue(squares, holes, session):
#     """Prints Queue to a file for displaying to the frontend

#     Args:
#         squares (list): list of squares returned from the get_queue method
#         holes (list): list of holes returned from the get_queue method
#         session (ScreeningSession): ScreeningSession object from Smartscope.server.models
#     """
#     string = ['------------------------------------------------------------\nCURRENT QUEUE:\n------------------------------------------------------------\nSquares:\n']
#     for s in squares:
#         string.append(f"\t{s.number} -> {s.name}\n")
#     string.append(f'------------------------------------------------------------\nHoles: (total={len(holes)})\n')
#     for h in holes:
#         string.append(f"\t{h.number} -> {h.name}\n")
#     string.append('------------------------------------------------------------\n')
#     string = ''.join(string)
#     with open(os.path.join(session.directory, 'queue.txt'), 'w') as f:
#         f.write(string)

