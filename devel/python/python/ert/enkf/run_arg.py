#  Copyright (C) 2014  Statoil ASA, Norway. 
#   
#  The file 'run_arg.py' is part of ERT - Ensemble based Reservoir Tool. 
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
from ert.enkf import ENKF_LIB, TimeMap, StateMap
from ert.enkf.enums import EnkfRunType, EnkfStateType



class RunArg(BaseCClass):
    def __init__(self , run_mode , iens , runpath , 
                 iter = 0 , 
                 parameters_init_step = 0 , 
                 parameters_init_state = EnkfStateType.ANALYZED, 
                 dynamic_init_state = EnkfStateType.ANALYZED, 
                 step1 = 0,
                 step2 = 0):

        if run_mode == EnkfRunType.ENSEMBLE_EXPERIMENT:
            c_ptr = RunArg.cNamespace().alloc_ENSEMBLE_EXPERIMENT(iens , iter , runpath)
        else:
            raise TypeError("Currently only run_mode == ENSEMBLE_EXPERIMENT can be instantiated from Python")
        super(RunArg , self).__init__(c_ptr)
        


    def free(self):
        RunArg.cNamespace().free(self)





cwrapper = CWrapper(ENKF_LIB)
cwrapper.registerType("run_arg", RunArg)
cwrapper.registerType("run_arg_obj", RunArg.createPythonObject)
cwrapper.registerType("run_arg_ref", RunArg.createCReference)



RunArg.cNamespace().alloc_ENSEMBLE_EXPERIMENT = cwrapper.prototype("c_void_p run_arg_alloc_ENSEMBLE_EXPERIMENT(int, int, char*)")
RunArg.cNamespace().free  = cwrapper.prototype("void run_arg_free(run_arg)")
