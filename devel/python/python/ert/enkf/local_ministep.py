from ert.cwrap import BaseCClass, CWrapper
from ert.enkf import ENKF_LIB, LocalObsdata, LocalObsdataNode, LocalDataset

class LocalMinistep(BaseCClass):

    def __init__(self, ministep_key):
        raise NotImplementedError("Class can not be instantiated directly!")
   
    def addNode(self, node):
        assert isinstance(node, LocalObsdataNode)
        LocalMinistep.cNamespace().add_node(self,node)
        
    def attachObsset(self, obs_set):
        assert isinstance(obs_set, LocalObsdata)
        LocalMinistep.cNamespace().attach_obsset(self,obs_set)
        
    def attachDataset(self, dataset):
        assert isinstance(dataset, LocalDataset)
        LocalMinistep.cNamespace().attach_dataset(self,dataset)
  
        
    def getLocalObsData(self):
        return LocalMinistep.cNamespace().get_local_obs_data(self)
    
    def getName(self):
        """ @rtype: str """
        return LocalMinistep.cNamespace().name(self) 
        
        
    def free(self):
        LocalMinistep.cNamespace().free(self) 

cwrapper = CWrapper(ENKF_LIB)
cwrapper.registerType("local_ministep", LocalMinistep)
cwrapper.registerType("local_ministep_obj", LocalMinistep.createPythonObject)
cwrapper.registerType("local_ministep_ref", LocalMinistep.createCReference)

LocalMinistep.cNamespace().alloc               = cwrapper.prototype("c_void_p local_ministep_alloc(char*)")
LocalMinistep.cNamespace().add_node            = cwrapper.prototype("void local_ministep_add_obsdata_node(local_ministep,local_obsdata_node)")
LocalMinistep.cNamespace().get_local_obs_data  = cwrapper.prototype("local_obsdata_ref local_ministep_get_obsdata(local_ministep)")
LocalMinistep.cNamespace().free                = cwrapper.prototype("void local_ministep_free(local_ministep)")
LocalMinistep.cNamespace().attach_obsset       = cwrapper.prototype("void local_ministep_add_obsdata(local_ministep,local_obsdata)")
LocalMinistep.cNamespace().attach_dataset      = cwrapper.prototype("void local_ministep_add_dataset(local_ministep,local_dataset)")
LocalMinistep.cNamespace().name                = cwrapper.prototype("char* local_ministep_get_name(local_ministep)")

