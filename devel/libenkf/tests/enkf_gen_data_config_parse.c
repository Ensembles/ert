/*
   Copyright (C) 2013  Statoil ASA, Norway. 
    
   The file 'enkf_gen_data_config_parse.c' is part of ERT - Ensemble based Reservoir Tool. 
    
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
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

#include <ert/util/test_work_area.h>
#include <ert/util/test_util.h>

#include <ert/config/config.h>

#include <ert/enkf/gen_data_config.h>
#include <ert/enkf/enkf_config_node.h>
#include <ert/enkf/config_keys.h>


enkf_config_node_type * parse_alloc_GEN_PARAM( const char * config_string , bool parse_valid) {
  config_type * config = config_alloc();
  enkf_config_node_type * enkf_config_node = NULL;
  
  enkf_config_node_add_GEN_PARAM_config_schema( config );
  {
    FILE * stream = util_fopen("config.txt" , "w");
    fprintf(stream , config_string);
    fclose( stream );
  }
  
  test_assert_bool_equal( parse_valid , config_parse( config , "config.txt" , "--" , NULL , NULL , CONFIG_UNRECOGNIZED_IGNORE , true));
  if (parse_valid) {
    const config_content_item_type * config_item = config_get_content_item( config , GEN_PARAM_KEY );
    const config_content_node_type * config_node = config_content_item_iget_node( config_item , 0 );

    enkf_config_node = enkf_config_node_alloc_GEN_PARAM_from_config( config_node );
  }
  config_free( config );
  return enkf_config_node;
}




void test_parse_gen_param() {
  test_work_area_type * work_area = test_work_area_alloc("GEN_PARAM_parse");

  // Parse error: missing eclfile
  {
    enkf_config_node_type * config_node = parse_alloc_GEN_PARAM( "GEN_PARAM KEY\n" , false);
    test_assert_NULL( config_node );
  }

  // Missing all required KEY: arguments
  {
    enkf_config_node_type * config_node = parse_alloc_GEN_PARAM( "GEN_PARAM KEY ECLFILE\n" , true);
    test_assert_NULL( config_node );
  }


  // OUTPUT_FORMAT: Is incorrectly spelled
  {
    enkf_config_node_type * config_node = parse_alloc_GEN_PARAM( "GEN_PARAM KEY ECLFILE INIT_FILES:XXX INPUT_FORMAT:ASCII OutPutFOrmat:ASCII\n" , true);
    test_assert_NULL( config_node );
  }


  // OUTPUT_FORMAT: ASCII is incorrectly spelled
  {
    enkf_config_node_type * config_node = parse_alloc_GEN_PARAM( "GEN_PARAM KEY ECLFILE INIT_FILES:XXX INPUT_FORMAT:ASCII OUTPUT_FORMAT:ASCI\n" , true);
    test_assert_NULL( config_node );
  }

  // Invalid value for INPUT_FORMAT
  {
    enkf_config_node_type * config_node = parse_alloc_GEN_PARAM( "GEN_PARAM KEY ECLFILE INIT_FILES:XXX INPUT_FORMAT:ASCII_TEMPLATE OUTPUT_FORMAT:ASCII\n" , true);
    test_assert_NULL( config_node );
  }


  // Correct
  {
    enkf_config_node_type * config_node = parse_alloc_GEN_PARAM( "GEN_PARAM KEY ECLFILE INPUT_FORMAT:BINARY_DOUBLE OUTPUT_FORMAT:ASCII INIT_FILES:INIT%%d\n" , true);

    test_assert_string_equal( "ECLFILE" , enkf_config_node_get_enkf_outfile( config_node ));
    test_assert_NULL(  enkf_config_node_get_enkf_infile( config_node ));
    test_assert_string_equal( "INIT%d" , enkf_config_node_get_init_file_fmt( config_node ));
    {
      gen_data_config_type * gen_data_config = enkf_config_node_get_ref( config_node );
      test_assert_int_equal( BINARY_DOUBLE , gen_data_config_get_input_format( gen_data_config ));
      test_assert_int_equal( ASCII         , gen_data_config_get_output_format( gen_data_config ));
    }
    
    enkf_config_node_free( config_node );
  }

  test_work_area_free( work_area );
}



int main(int argc , char ** argv) {
  test_parse_gen_param();
  exit(0);
}

