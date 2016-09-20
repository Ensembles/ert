from ert.config import ContentTypeEnum , ConfigPrototype
from ert.cwrap import BaseCClass


class SchemaItem(BaseCClass):
    TYPE_NAME = "schema_item"

    _alloc = ConfigPrototype("void* config_schema_item_alloc( char* , bool )", bind=False)
    _free = ConfigPrototype("void config_schema_item_free( schema_item )")
    _iget_type = ConfigPrototype("config_content_type_enum config_schema_item_iget_type( schema_item, int)")
    _iset_type = ConfigPrototype("void config_schema_item_iset_type( schema_item , int , config_content_type_enum)")
    _set_argc_minmax = ConfigPrototype("void config_schema_item_set_argc_minmax( schema_item , int , int)")

    def __init__(self, keyword, required=False):
        c_ptr = self._alloc(keyword, required)
        super(SchemaItem, self).__init__(c_ptr)


    def iget_type( self, index):
        """ @rtype: ContentTypeEnum """
        return self._iget_type(index)

    def iset_type( self, index, schema_type ):
        """
        @type schema_type: ContentTypeEnum
        """
        assert isinstance(schema_type, ContentTypeEnum)
        self._iset_type(index, schema_type)

    def set_argc_minmax(self, minimum, maximum):
        self._set_argc_minmax(minimum, maximum)

    def free(self):
        self._free()

