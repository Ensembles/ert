/*
   Copyright (C) 2015  Statoil ASA, Norway.

   The file 'config_content.c' is part of ERT - Ensemble based Reservoir Tool.

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

#include <ert/util/test_util.h>

#include <ert/config/config_content.h>


void test_create() {
  config_content_type * content = config_content_alloc( );
  test_assert_true( config_content_is_instance( content ) );
  config_content_free( content );
}






int main( int argc , char ** argv) {
  test_create();
}
