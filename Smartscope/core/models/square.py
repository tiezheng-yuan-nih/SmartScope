from .base_model import *
from .target import Target
from .extra_property_mixin import ExtraPropertyMixin
from .atlas import AtlasModel
from .hole import HoleModel
from .grid import AutoloaderGrid

from django.apps import apps
from Smartscope.core.svg_plots import drawSquare


class ChangeLog(BaseModel):
    table_name = models.CharField(max_length=60)
    grid_id = models.ForeignKey(
        AutoloaderGrid,
        on_delete=models.CASCADE,
        to_field='grid_id'
    )
    line_id = models.CharField(max_length=30)
    column_name = models.CharField(max_length=20)
    initial_value = models.BinaryField()
    new_value = models.BinaryField()
    date = models.DateTimeField(auto_now=True)
    user = models.ForeignKey(
        User,
        to_field='username',
        on_delete=models.SET_NULL,
        null=True,
        default=None
    )

    class Meta(BaseModel.Meta):
        db_table = 'changelog'

    @ property
    def table_model(self):
        for model in apps.get_models():
            if model._meta.db_table == self.table_name:
                return model
            
class ImageManager(models.Manager):
    use_for_related_fields = True

    def get_queryset(self):
        return super().get_queryset().prefetch_related('grid_id__session_id')

class SquareDisplayManager(models.Manager):

    def get_queryset(self):
        return super().get_queryset().prefetch_related('finders')\
            .prefetch_related('classifiers').prefetch_related('selectors')\
            .prefetch_related('holemodel_set')

class SquareImageManager(models.Manager):
    def get_queryset(self):
        return super().get_queryset().prefetch_related('grid_id__session_id')\
            .prefetch_related('holemodel_set')

class SquareModel(Target, ExtraPropertyMixin):
    square_id = models.CharField(max_length=30, primary_key=True, editable=False)
    area = models.FloatField(null=True)
    atlas_id = models.ForeignKey(
        AtlasModel,
        on_delete=models.CASCADE,
        to_field='atlas_id'
    )

    # Managers
    withholes = SquareImageManager()
    objects = ImageManager()
    display = SquareDisplayManager()
    # aliases

    @ property
    def alias_name(self):
        return f'Area {self.number}'

    @ property
    def api_viewset_name(self):
        return 'squares'

    @property
    def prefix(self):
        return 'Square'

    @ property
    def targets_prefix(self):
        return 'hole'

    @ property
    def id(self):
        return self.square_id

    @ property
    def parent(self):
        return self.atlas_id

    @ parent.setter
    def set_parent(self, parent):
        self.atlas_id = parent
    # endaliases

    @ property
    def parent_stage_z(self):
        return self.parent.stage_z

    @ property
    def targets(self):
        return self.holemodel_set.all()


    # @cached_model_property(key_prefix='svg', 
    # extra_suffix_from_function=['method'], timeout=3600)
    def svg(self, display_type, method):
        holes = list(HoleModel.display.filter(square_id=self.square_id))
        sq = drawSquare(self, holes, display_type, method)
        
        return sq

    @ property
    def has_queued(self):
         return self.holemodel_set(manager='just_holes').\
            filter(status='queued').exists()

    @ property
    def has_completed(self):
        return self.holemodel_set(manager='just_holes').\
            filter(status='completed').exists()

    @ property
    def has_active(self):
        return self.holemodel_set(manager='just_holes').\
            filter(status__in=['acquired', 'processed', 'targets_picked', 'started']).\
            exists()


    @ property
    def initial_quality(self):
        try:
            return ChangeLog.objects\
                .get(grid_id=self.grid_id, line_id=self.hole_id, column_name='quality')\
                .initial_value
        except:
            return self.quality

    @ property
    def extracted_file(self):
        logger.debug('Getting extracted file')

    class Meta(BaseModel.Meta):
        unique_together = ('name', 'atlas_id')
        db_table = 'squaremodel'

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        if not self.square_id:
            self.name = f'{self.grid_id.name}_square{self.number}'
            self.square_id = generate_unique_id(extra_inputs=[self.name[:20]])
        self.raw = os.path.join('raw', f'{self.name}.mrc')

    def save(self, *args, **kwargs):

        super().save(*args, **kwargs)
        return self

    def __str__(self):
        return self.name