#  Copyright (C) 2011  Statoil ASA, Norway.
#
#  The file 'export_model.py' is part of ERT - Ensemble based Reservoir Tool.
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
from ert.enkf import EnkfVarType
from ert_gui.models import ErtConnector

class ExportModel(ErtConnector):

    def __init__(self):
        super(ExportModel, self).__init__()

    def exportField(self, keyword, path, iactive, file_type, report_step, state, selected_case):
        """
        @type keyword: str
        @type path: str
        @type iactive: BoolVector
        @type file_type: EnkfFieldFileFormatEnum
        @type report_step: int
        @type state: EnkfStateType
        @type selected_case: str

        """
        fs = self.ert().getEnkfFsManager().getFileSystem(selected_case)
        return self.ert().exportField(keyword, path, iactive, file_type, report_step, state, fs)


