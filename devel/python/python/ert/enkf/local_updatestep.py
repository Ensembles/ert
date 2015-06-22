from ert.cwrap import BaseCClass, CWrapper
from ert.enkf import ENKF_LIB, LocalMinistep

class LocalUpdateStep(BaseCClass):

    def __init__(self, updatestep_key):
        assert isinstance(updatestep_key, str)

        c_pointer = LocalUpdateStep.cNamespace().alloc(updatestep_key)
        super(LocalUpdateStep, self).__init__(c_pointer)
          
    def attachMinistep(self, ministep):
        assert isinstance(ministep, LocalMinistep)
        LocalUpdateStep.cNamespace().attach_ministep(self,ministep)
        
               
    def free(self):
        LocalUpdateStep.cNamespace().free(self) 

cwrapper = CWrapper(ENKF_LIB)
cwrapper.registerType("local_updatestep", LocalUpdateStep)
cwrapper.registerType("local_updatestep_obj", LocalUpdateStep.createPythonObject)
cwrapper.registerType("local_updatestep_ref", LocalUpdateStep.createCReference)

LocalUpdateStep.cNamespace().alloc               = cwrapper.prototype("c_void_p local_updatestep_alloc(char*)")
LocalUpdateStep.cNamespace().free                = cwrapper.prototype("void local_updatestep_free(local_updatestep)")
LocalUpdateStep.cNamespace().attach_ministep     = cwrapper.prototype("void local_updatestep_add_ministep(local_updatestep,local_ministep)")



