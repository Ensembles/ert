#  Copyright (C) 2016  Statoil ASA, Norway.
#
#  The file 'ert_init_context.py' is part of ERT - Ensemble based Reservoir Tool.
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

import os.path
from ert.cwrap import BaseCClass
from ert.enkf import EnkfFs, EnkfPrototype


class ErtInitContext(BaseCClass):
    TYPE_NAME = "ert_init_context" 

    _alloc = EnkfPrototype("void* ert_init_context_alloc( enkf_fs, bool_vector , path_fmt , subst_list , enkf_init_mode_enum , int) " , bind = False)
    _free  = EnkfPrototype("void  ert_init_context_free( ert_init_context )")


    def __init__(self , init_fs , iactive , path_fmt , subst_list , init_mode , iter_nr):
        c_ptr = self._alloc( init_fs , iactive , path_fmt , subst_list , init_mode , iter_nr)
        super(ErtInitContext, self).__init__( c_ptr )


    def free(self):
        self._free( )
