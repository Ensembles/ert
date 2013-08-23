#  Copyright (C) 2013  Statoil ASA, Norway. 
#   
#  The file 'history.py' is part of ERT - Ensemble based Reservoir Tool. 
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
from ert.cwrap import CWrapper, BaseCClass
from ert.sched import SCHED_LIB, SchedFile
from ert.ecl import EclSum


class HistoryType(BaseCClass):
    def __init__(self):
        raise NotImplementedError("Class can not be instantiated directly!")

    def get_source_string(self):
        """ @rtype: str """
        return HistoryType.cNamespace().get_source_string(self)

    #todo: change this to __init__?
    @staticmethod
    def alloc_from_refcase(refcase, use_history):
        """
        @type refcase: EclSum
        @type use_history: bool
        @rtype: HistoryType
        """
        return HistoryType.cNamespace().alloc_from_refcase(refcase, use_history)

    @staticmethod
    def alloc_from_sched_file(sched_file):
        """ @rtype: HistoryType """
        assert isinstance(sched_file, SchedFile)
        return HistoryType.cNamespace().alloc_from_sched_file(":", sched_file)

    def free(self):
        HistoryType.cNamespace().free(self)


cwrapper = CWrapper(SCHED_LIB)
cwrapper.registerType("history_type", HistoryType)
cwrapper.registerType("history_type_obj", HistoryType.createPythonObject)
cwrapper.registerType("history_type_ref", HistoryType.createCReference)

HistoryType.cNamespace().free = cwrapper.prototype("void history_free( history_type )")
HistoryType.cNamespace().get_source_string = cwrapper.prototype("char* history_get_source_string(history_type)")
HistoryType.cNamespace().alloc_from_refcase = cwrapper.prototype("history_type_obj history_alloc_from_refcase(ecl_sum, bool)")
HistoryType.cNamespace().alloc_from_sched_file = cwrapper.prototype("history_type_obj history_alloc_from_sched_file(char*, sched_file)")
