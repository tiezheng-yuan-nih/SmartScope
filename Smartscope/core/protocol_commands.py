from Smartscope.lib.transformations import add_IS_offset
from math import cos, radians
import numpy as np
import logging

logger = logging.getLogger(__name__)

def setAtlasOptics(scope,params,instance) -> None:
    """Set the microscope mag, spot size and C2 current for the atlas based on the chosen detector."""
    scope.set_atlas_optics()

def atlas(scope,params,instance) -> None:
    """Collects and atlas of X by Y tiles from the collection parameters using the Montage command"""
    scope.atlas(size=[params.atlas_x,params.atlas_y],file=instance.raw)

def realignToSquare(scope,params,instance) -> None:
    """Realigns to the square using the Search magnification. 
    Mainly useful when the alignement between the Atlas and the Square is off.
    """
    from Smartscope.core.db_manipulations import set_or_update_refined_finder
    stageX, stageY, stageZ = scope.realign_to_square()
    set_or_update_refined_finder(instance.square_id, stageX, stageY, stageZ)
    scope.moveStage(stageX,stageY,stageZ)   

def square(scope,params,instance) -> None:
    """Acquires and save the square image using the Search preset."""
    scope.square(file=instance.raw)

def moveStage(scope,params,instance) -> None:
    """Moves the stage to the instance position"""
    finder = instance.finders.first()
    stage_x, stage_y, stage_z = finder.stage_x, finder.stage_y, finder.stage_z
    scope.moveStage(stage_x,stage_y,stage_z)

def eucentricSearch(scope,params,instance):
    """Calculates eucentricity using the Search preset. 
    
    This is faster than using the built-in serialEM Eucentricity by not having to switch the optics back and forth between View and Search. Will speed up screening.
    
    However, this is less precise than Eucentricty and should be avoided when using Falcon detectors or when setting up for data collection.
    """
    scope.eucentricHeight()

def eucentricMediumMag(scope,params,instance):
    """Calculates eucentricity using the View preset. Equivalent to Eucentric Rough."""
    scope.eucentricity()

def mediumMagHole(scope,params,instance):
    """Acquires the hole at the View preset."""
    scope.medium_mag_hole(file=instance.raw)

def tiltToAngle(scope,params,instance):
    scope.tiltTo(params.tilt_angle)

def alignToHoleRef(scope,params,instance):
    """Aligns the medium mag to the template hole image stored in buffer T. Either load an image manually or use the loadHoleRef command prior to this one."""
    while True:
        shift = scope.align_to_hole_ref()
        if np.sqrt(np.sum(np.array(shift)**2)) < 700:
            break
        scope.reset_image_shift()

def loadHoleRef(scope,params,instance):
    """Loads the references/holeref.mrc image into buffer T to be used as hole template for the alignToHoleRef command."""
    if scope.has_hole_ref:
        return
    scope.load_hole_ref()

def highMag(scope, params,instance):
    """Acquires the highmag image. 
    
    Will set image shift and the specificed offset from the collection parameters.
    
    Also automatically corrects defocus and image-shift based on the tilt angle.
    """
    finder = instance.finders.first()
    stage_x, stage_y, _ = scope.report_stage()
    grid = instance.grid_id
    grid_type = grid.holeType
    grid_mesh = grid.meshMaterial
    offset = 0
    if params.offset_targeting and (grid.collection_mode == 'screening' or params.offset_distance != -1) and grid_type.hole_size is not None:
        offset = add_IS_offset(grid_type.hole_size, grid_mesh.name, offset_in_um=params.offset_distance)
    isX, isY = stage_x - finder.stage_x + offset, (stage_y - finder.stage_y) * cos(radians(params.tilt_angle))
    logger.debug(f'The tilt angle is {params.tilt_angle}, Y axis image-shift corrected from {stage_y - finder.stage_y:.2f} to {isY:.2f}')
    scope.image_shift_by_microns(isX,isY,params.tilt_angle)
    frames = scope.highmag(file=instance.raw, frames=params.save_frames, earlyReturn=any([params.force_process_from_average, params.save_frames is False]))
    instance.is_x=isX
    instance.is_y=isY
    instance.offset=offset
    instance.frames=frames
    return instance    


protocolCommandsFactory = dict(
    setAtlasOptics=setAtlasOptics,
    atlas=atlas,
    realignToSquare=realignToSquare,
    square=square,
    moveStage=moveStage,
    eucentricSearch=eucentricSearch,
    eucentricMediumMag=eucentricMediumMag,
    mediumMagHole=mediumMagHole,
    tiltToAngle=tiltToAngle,
    alignToHoleRef=alignToHoleRef,
    loadHoleRef=loadHoleRef,
    highMag=highMag,
)