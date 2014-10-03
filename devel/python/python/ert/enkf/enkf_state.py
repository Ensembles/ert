#  Copyright (C) 2012  Statoil ASA, Norway. 
#   
#  The file 'enkf_state.py' is part of ERT - Ensemble based Reservoir Tool. 
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
from ert.enkf import ENKF_LIB
from ert.job_queue import JobStatusType


class EnKFState(BaseCClass):
    def __init__(self):
        raise NotImplementedError("Class can not be instantiated directly!")
        
    
    def __getitem__(self , kw):
        if isinstance(kw , str):
            if self.hasKey(kw):
                node = EnKFState.cNamespace().get_node( self , kw )
                node.setParent( self )
                return node
            else:
                raise KeyError("The state object does not have node:%s" % kw)
        else:
            raise TypeError("The kw type must be string. Input:%s" % kw)


    def hasKey(self , kw):
        return EnKFState.cNamespace().has_key( self , kw )


    def getNode(self , kw):
        return self[kw]


    def free(self):
        EnKFState.cNamespace().free(self)



cwrapper = CWrapper(ENKF_LIB)
cwrapper.registerType("enkf_state", EnKFState)
cwrapper.registerType("enkf_state_obj", EnKFState.createPythonObject)
cwrapper.registerType("enkf_state_ref", EnKFState.createCReference)


EnKFState.cNamespace().free     = cwrapper.prototype("void enkf_state_free( enkf_state )")
EnKFState.cNamespace().has_key  = cwrapper.prototype("bool enkf_state_has_node( enkf_state , char* )")
EnKFState.cNamespace().get_node = cwrapper.prototype("enkf_node_ref enkf_state_get_node( enkf_state , char* )")
