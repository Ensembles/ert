/*
   Copyright (C) 2013  Statoil ASA, Norway. 
    
   The file 'enkf_ecl_config.c' is part of ERT - Ensemble based Reservoir Tool. 
    
   ERT is free software: you can redistribute it and/or modify 
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation, either version 3 of the License, or 
   (at your option) any later version. 
    
   ERT is distributed in the hope that it will be useful, but WITHOUT ANY 
   WARRANTY; without even the implied warranty of MERCHANTABILITY or 
   FITNESS FOR A PARTICULAR PURPOSE.   
    
   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
   for more details. 
*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <ert/util/test_util.h>

#include <ert/enkf/ecl_config.h>


int main(int argc , char ** argv) {
  ecl_config_type * ecl_config = ecl_config_alloc_empty();
  
  if (argc == 2) 
    test_assert_true(ecl_config_load_refcase( ecl_config , argv[1]));
  
  test_assert_false(ecl_config_load_refcase( ecl_config , "DOES_NOT_EXIST" ));
  test_assert_true(ecl_config_load_refcase( ecl_config , NULL ));

  
  ecl_config_free( ecl_config );
  exit(0);
}

