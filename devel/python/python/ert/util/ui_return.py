#  Copyright (C) 2013  Statoil ASA, Norway. 
#   
#  The file 'ui_return.py' is part of ERT - Ensemble based Reservoir Tool. 
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
from ert.util import UtilPrototype
from enums import UIReturnStatusEnum
    

class UIReturn(BaseCClass):
    TYPE_NAME = "ui_return"
    _alloc = UtilPrototype("void* ui_return_alloc( ui_return_status )")
    _free = UtilPrototype("void ui_return_free(ui_return)")
    _get_status = UtilPrototype("ui_return_status ui_return_get_status(ui_return)")
    _get_help = UtilPrototype("char* ui_return_get_help(ui_return)")
    _add_help = UtilPrototype("void ui_return_add_help(ui_return)")
    _add_error = UtilPrototype("void ui_return_add_error(ui_return)")
    _num_error = UtilPrototype("int ui_return_get_error_count(ui_return)")
    _last_error = UtilPrototype("char* ui_return_get_last_error(ui_return)")
    _first_error = UtilPrototype("char* ui_return_get_first_error(ui_return)")
    _iget_error = UtilPrototype("char* ui_return_iget_error(ui_return , int)")
    
    def __init__(self , status):
        c_ptr = self._alloc(status)
        super(UIReturn, self).__init__(c_ptr)


    def __nonzero__(self):
        if self.status() == UIReturnStatusEnum.UI_RETURN_OK:
            return True
        else:
            return False
        
        
    def __len__(self):
        return self._num_error(self)
    

    def __getitem__(self , index):
        if isinstance(index , int):
            if 0 <= index < len(self):
                return self._iget_error(self , index)
            else:
                raise IndexError
        else:
            raise TypeError("Lookup type must be integer")
  
        
    def iget_error(self , index):
        return self.__getitem__(index)
  
        
    def help_text(self):
        help_text = self._get_help(self)
        if help_text:
            return help_text
        else:
            return ""
        
    def add_help(self, help_text):
        self._add_help(self , help_text)
        
        
    def status(self):
        return self._get_status(self)
        
    
    def __assert_error(self):
        if self.status() == UIReturnStatusEnum.UI_RETURN_OK:
            raise ValueError("Can not add error messages to object in state RETURN_OK")
    
    
    def add_error(self, error):
        self.__assert_error()
        self._add_error(self , error)

    
    def last_error(self):
        self.__assert_error()
        return self._last_error(self)
    
    
    def first_error(self):
        self.__assert_error()
        return self._first_error(self)
    

    def free(self):
        self._free(self)
