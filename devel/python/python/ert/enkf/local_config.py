#  Copyright (C) 2012  Statoil ASA, Norway. 
#   
#  The file 'local_config.py' is part of ERT - Ensemble based Reservoir Tool. 
#   
#  ERT is free software: you can redistribute it and/or modify 
#  it under the terms of the GNU General Public License as published by 
#  the Free Software Foundation, either version 3 of the License, or 
#  (at your option) any later version. 
#   
#  ERT is distributed in the hope that it will be useful, but WITHOUT ANY 
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or 
#  FITNESS FOR A PARTICULAR PURPOSE.   
#   
#  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
#  for more details.
from ert.cwrap import BaseCClass, CWrapper
from ert.enkf import ENKF_LIB, LocalUpdateStep

from ert.util import StringList
from ert.enkf.local_ministep import LocalMinistep


class LocalConfig(BaseCClass):
    
    def __init__(self):
        raise NotImplementedError("Class can not be instantiated directly!")
        
    def free(self):
        LocalConfig.cNamespace().free(self)

    def getConfigFiles(self):
        """ @rtype: StringList """
        return LocalConfig.cNamespace().get_config_files(self).setParent(self)

    def clearConfigFiles(self):
        LocalConfig.cNamespace().clear_config_files(self)

    def addConfigFile(self, filename):
        LocalConfig.cNamespace().add_config_file(self, filename)
        
    def writeLocalConfigFile(self, filename):
        LocalConfig.cNamespace().write_local_config_file(self, filename)
           
    def createUpdatestep(self, update_step_key):
        LocalConfig.cNamespace().create_updatestep(self, update_step_key)  
        return LocalConfig.cNamespace().get_updatestep(self, update_step_key)  
    
    def getUpdatestep(self, update_step_key):
        return LocalConfig.cNamespace().get_updatestep(self, update_step_key) 
                
    def installUpdatestep(self, update_step, step1, step2):
        assert isinstance(update_step, LocalUpdateStep)
        LocalConfig.cNamespace().set_updatestep(self, step1, step2, update_step.getName())     
                
    def createMinistep(self, mini_step_key):
        LocalConfig.cNamespace().create_ministep(self, mini_step_key)         
        return LocalConfig.cNamespace().get_ministep(self, mini_step_key)  
        
    def attachMinistep(self, update_step, mini_step):
        assert isinstance(mini_step, LocalMinistep)
        assert isinstance(update_step, LocalUpdateStep)
        LocalConfig.cNamespace().attach_ministep(update_step, mini_step)           
        



cwrapper = CWrapper(ENKF_LIB)
cwrapper.registerType("local_config", LocalConfig)
cwrapper.registerType("local_config_obj", LocalConfig.createPythonObject)
cwrapper.registerType("local_config_ref", LocalConfig.createCReference)

LocalConfig.cNamespace().free                    = cwrapper.prototype("void local_config_free( local_config )")
LocalConfig.cNamespace().get_config_files        = cwrapper.prototype("stringlist_ref local_config_get_config_files( local_config )")
LocalConfig.cNamespace().clear_config_files      = cwrapper.prototype("void local_config_clear_config_files( local_config )")
LocalConfig.cNamespace().add_config_file         = cwrapper.prototype("void local_config_add_config_file( local_config , char*)")
LocalConfig.cNamespace().write_local_config_file = cwrapper.prototype("void local_config_fprintf( local_config, char*)")

LocalConfig.cNamespace().get_updatestep          = cwrapper.prototype("local_updatestep_ref local_config_get_updatestep( local_config, char*)")
LocalConfig.cNamespace().create_updatestep       = cwrapper.prototype("void local_config_alloc_updatestep( local_config, char*)")
LocalConfig.cNamespace().set_updatestep          = cwrapper.prototype("void local_config_set_updatestep( local_config, int, int, char*)")

LocalConfig.cNamespace().get_ministep            = cwrapper.prototype("local_ministep_ref local_config_get_ministep( local_config, char*)")
LocalConfig.cNamespace().create_ministep         = cwrapper.prototype("void local_config_alloc_ministep( local_config, char*)")
LocalConfig.cNamespace().attach_ministep         = cwrapper.prototype("void local_updatestep_add_ministep( local_updatestep, local_ministep)")











