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

from cwrap import BaseCClass
from ert.sched import SchedFile, HistorySourceEnum, SchedulePrototype
from ert.ecl import EclSum

class History(BaseCClass):
    TYPE_NAME = "history"
    # cwrapper.registerType("history_obj", History.createPythonObject)
    # cwrapper.registerType("history_ref", History.createCReference)

    _alloc_from_refcase    = SchedulePrototype("void* history_alloc_from_refcase(ecl_sum, bool)", bind = False)
    _alloc_from_sched_file = SchedulePrototype("void* history_alloc_from_sched_file(char*, sched_file)", bind = False)
    _get_source_string     = SchedulePrototype("char* history_get_source_string(history_source_enum)", bind = False)
    _free                  = SchedulePrototype("void  history_free( history )")
    # _history_get_source_type = SchedulePrototype("history_source_type_enum history_get_source_type(char*)", bind = False)

    def __init__(self, refcase = None, use_history = False, sched_file = None):
        """
        @type refcase: EclSum
        @type use_history: bool
        @rtype: HistoryType
        """
        if sched_file is not None:
            c_ptr = self._alloc_from_sched_file(refcase, use_history)
        else:
            c_ptr = self._alloc_from_refcase(refcase, use_history)
        if c_ptr:
            super(History, self).__init__(c_ptr)
        else:
            if sched_file is None and refcase is None:
                raise ValueError('Need to specify either sched_file or refcase.')
            raise ValueError('Invalid input.  Failed to create History.')

    @staticmethod
    def get_source_string(history_source_type):
        """
        @type history_source_type: HistorySourceEnum
        @rtype: str
        """
        return self._get_source_string(history_source_type)

    def free(self):
        self._free( self )
