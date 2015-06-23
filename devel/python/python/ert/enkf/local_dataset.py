from ert.cwrap import BaseCClass, CWrapper
from ert.enkf import ENKF_LIB


class Localdataset(BaseCClass):

    def __init__(self, name):
        assert isinstance(name, str)

        c_pointer = Localdataset.cNamespace().alloc(name)
        super(Localdataset, self).__init__(c_pointer)
                 
    def getName(self):
        """ @rtype: str """
        return Localdataset.cNamespace().name(self)
    
    def getActiveList(self, key):
        """ @rtype: str """
        return Localdataset.cNamespace().active_list(self, key)

    def free(self):
        Localdataset.cNamespace().free(self)



cwrapper = CWrapper(ENKF_LIB)
cwrapper.registerType("local_dataset", Localdataset)
cwrapper.registerType("local_dataset_obj", Localdataset.createPythonObject)
cwrapper.registerType("local_dataset_ref", Localdataset.createCReference)

Localdataset.cNamespace().alloc          = cwrapper.prototype("c_void_p local_dataset_alloc(char*)")
Localdataset.cNamespace().free           = cwrapper.prototype("void local_dataset_free(local_dataset)")
Localdataset.cNamespace().name           = cwrapper.prototype("char* local_dataset_get_name(local_dataset)")
Localdataset.cNamespace().active_list    = cwrapper.prototype("active_list_ref local_dataset_get_node_active_list(local_dataset, char*)")




