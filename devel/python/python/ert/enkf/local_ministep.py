from ert.cwrap import BaseCClass, CWrapper
from ert.enkf import ENKF_LIB

class LocalMinistep(BaseCClass):

    def __init__(self, ministep_key):
        assert isinstance(ministep_key, str)

        c_pointer = LocalMinistep.cNamespace().alloc(ministep_key)
        super(LocalMinistep, self).__init__(c_pointer)
   
    def free(self):
        LocalMinistep.cNamespace().free(self)


#     def addObsVector(self , obs_vector):
#         self.addNode( obs_vector.createLocalObs() )
 

cwrapper = CWrapper(ENKF_LIB)
cwrapper.registerObjectType("local_ministep", LocalMinistep)

LocalMinistep.cNamespace().alloc = cwrapper.prototype("c_void_p local_ministep_alloc(char*)")
LocalMinistep.cNamespace().free  = cwrapper.prototype("void local_ministep_free(local_ministep)")



