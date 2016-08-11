#  Copyright (C) 2013  Statoil ASA, Norway.
#
#  The file 'content_type_enum.py' is part of ERT - Ensemble based Reservoir Tool.
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
from ert.cwrap import BaseCEnum
from ert.enkf import ENKF_LIB


class EnkfInitModeEnum(BaseCEnum):
    INIT_NONE = None
    INIT_CONDITIONAL = None
    INIT_FORCE = None


EnkfInitModeEnum.addEnum("INIT_NONE", 0)
EnkfInitModeEnum.addEnum("INIT_CONDITIONAL", 1)
EnkfInitModeEnum.addEnum("INIT_FORCE", 2)
EnkfInitModeEnum.registerEnum(ENKF_LIB, "enkf_init_mode_enum")



