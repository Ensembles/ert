/*
   Copyright (C) 2014  Statoil ASA, Norway. 
    
   The file 'ert_run_context.c' is part of ERT - Ensemble based Reservoir Tool. 
   
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

#include <ert/util/type_macros.h>
#include <ert/util/vector.h>

#include <ert/enkf/enkf_types.h>
#include <ert/enkf/run_arg.h>
#include <ert/enkf/ert_run_context.h>

#define ERT_RUN_CONTEXT_TYPE_ID 55534132


struct ert_run_context_struct {
  UTIL_TYPE_ID_DECLARATION;
  vector_type      * run_args;
  bool_vector_type * iactive;
  run_mode_type      run_mode;
  int                step1;
  int                step2;
};



/*
  run_mode_type    run_mode        ,
  bool_vector_type * iactive ,
  int load_start                   ,      
  int init_step_parameter          ,
  state_enum init_state_parameter  ,
  state_enum init_state_dynamic    ,
  int iter                         ,
  int step1                        ,
  int step2) 
*/

ert_run_context_type * ert_run_context_alloc(bool_vector_type * iactive , 
                                             run_mode_type run_mode , 
                                             int load_start , 
                                             int init_step_parameter , 
                                             state_enum init_state_parameter,
                                             state_enum init_state_dynamic , 
                                             int iter , 
                                             int step1 , 
                                             int step2 ) {
    
  ert_run_context_type * context = util_malloc( sizeof * context );
  UTIL_TYPE_ID_INIT( context , ERT_RUN_CONTEXT_TYPE_ID );
  context->run_mode = run_mode;
  context->iactive = bool_vector_alloc_copy( iactive );
  context->step1 = step1;
  context->step2 = step2;
  context->run_args = vector_alloc_new();
  

  for (int iens = 0; iens < bool_vector_size( iactive ); iens++) {
    if (bool_vector_iget( iactive , iens )) {
      //run_arg_type * arg = run_arg_alloc_deprecated( );
      run_arg_type * arg = run_arg_alloc( iens , run_mode , init_step_parameter , init_state_parameter , init_state_dynamic , load_start , step1 , step2 , iter , "/path/to/run" );
      vector_append_owned_ref( context->run_args , arg , run_arg_free__);
    }
  }
  
  return context;
}



UTIL_IS_INSTANCE_FUNCTION( ert_run_context , ERT_RUN_CONTEXT_TYPE_ID );



void ert_run_context_free( ert_run_context_type * context ) {
  vector_free( context->run_args );
  bool_vector_free( context->iactive );
  free( context );
}

int ert_run_context_get_size( const ert_run_context_type * context ) {
  return vector_get_size( context->run_args );
}



run_mode_type ert_run_context_get_mode( const ert_run_context_type * context ) {
  return context->run_mode;
}


int ert_run_context_get_step1( const ert_run_context_type * context ) {
  return context->step1;
}


int ert_run_context_get_step2( const ert_run_context_type * context ) {
  return context->step2;
}


const bool_vector_type * ert_run_context_get_iactive( const ert_run_context_type * context ) {
  return context->iactive;
}
