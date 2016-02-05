#  Copyright (C) 2015  Statoil ASA, Norway. 
#   
#  The file 'arg_pack.py' is part of ERT - Ensemble based Reservoir Tool. 
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

from ert.cwrap import BaseCClass
from ert.util import BoundUtilPrototype, UtilPrototype


class ArgPack(BaseCClass):
    TYPE_NAME = "arg_pack"

    _alloc = UtilPrototype("void* arg_pack_alloc()")
    _append_int = UtilPrototype("void arg_pack_append_int(arg_pack, int)")
    _append_double = UtilPrototype("void arg_pack_append_double(arg_pack, double)")
    _append_ptr = UtilPrototype("void arg_pack_append_ptr(arg_pack, void*)")

    __len__ = BoundUtilPrototype("int arg_pack_size(arg_pack)")
    free = BoundUtilPrototype("void arg_pack_free(arg_pack)")

    def __init__(self, *args):
        c_ptr = self._alloc()
        super(ArgPack, self).__init__(c_ptr)
        self.child_list = []
        for arg in args:
            self.append(arg)

    def append(self, data):
        if isinstance(data, int):
            self._append_int(self, data)
        elif isinstance(data, float):
            self._append_double(self, data)
        elif isinstance(data, BaseCClass):
            self._append_ptr(self, BaseCClass.from_param(data))
            self.child_list.append(data)
        else:
            raise TypeError("Can only add int/double/basecclass")
