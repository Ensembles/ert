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
#include <ert/util/path_fmt.h>
#include <ert/util/subst_list.h>
#include <ert/util/int_vector.h>
#include <ert/util/type_vector_functions.h>

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
  int_vector_type  * iens_map;
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

ert_run_context_type * ert_run_context_alloc(const bool_vector_type * iactive , 
                                             path_fmt_type * runpath_fmt , 
                                             subst_list_type * subst_list , 
                                             run_mode_type run_mode , 
                                             int init_step_parameter , 
                                             state_enum init_state_parameter,
                                             state_enum init_state_dynamic , 
                                             int iter , 
                                             int step1 , 
                                             int step2 ) {
    
  ert_run_context_type * context = util_malloc( sizeof * context );
  UTIL_TYPE_ID_INIT( context , ERT_RUN_CONTEXT_TYPE_ID );

  context->iactive = bool_vector_alloc_copy( iactive );
  context->iens_map = bool_vector_alloc_active_index_list( iactive , -1 );
  context->run_args = vector_alloc_new();
  context->run_mode = run_mode;
  
  context->step1 = step1;
  context->step2 = step2;
  
  
  for (int iens = 0; iens < bool_vector_size( iactive ); iens++) {
    if (bool_vector_iget( iactive , iens )) {
      char * tmp1 = path_fmt_alloc_path(runpath_fmt , false , iens, iter);    /* 1: Replace first %d with iens, if a second %d replace with iter */
      char * tmp2 = tmp1;

      if (subst_list)
        tmp2 = subst_list_alloc_filtered_string( subst_list , tmp1 );         /* 2: Filter out various magic strings like <CASE> and <CWD>. */
      {
        run_arg_type * arg;

        switch (run_mode) {
        case(ENSEMBLE_EXPERIMENT):
          arg = run_arg_alloc_ENSEMBLE_EXPERIMENT( iens , iter , tmp2);
          break;
        case(INIT_ONLY):
          arg = run_arg_alloc_INIT_ONLY( iens , iter , tmp2);
          break;
        default:
          arg = run_arg_alloc( iens , run_mode , init_step_parameter , init_state_parameter , init_state_dynamic , step1 , step2 , iter , tmp2);
          break;
        }

        vector_append_owned_ref( context->run_args , arg , run_arg_free__);
      }

      if (subst_list)
        free( tmp2 );
      free( tmp1 );
    }
  }
  
  return context;
}



ert_run_context_type * ert_run_context_alloc_ENSEMBLE_EXPERIMENT(const bool_vector_type * iactive , 
                                                                 path_fmt_type * runpath_fmt , 
                                                                 subst_list_type * subst_list ,
                                                                 int iter) {
  return ert_run_context_alloc( iactive , runpath_fmt , subst_list , ENSEMBLE_EXPERIMENT , 0 , ANALYZED , ANALYZED , iter , 0 , 0 );
}



UTIL_IS_INSTANCE_FUNCTION( ert_run_context , ERT_RUN_CONTEXT_TYPE_ID );



void ert_run_context_free( ert_run_context_type * context ) {
  vector_free( context->run_args );
  bool_vector_free( context->iactive );
  int_vector_free( context->iens_map );
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


run_arg_type * ert_run_context_iget_arg( const ert_run_context_type * context , int index) {
  return vector_iget( context->run_args , index );
}


run_arg_type * ert_run_context_iens_get_arg( const ert_run_context_type * context , int iens) {
  int index = int_vector_iget( context->iens_map , iens );
  if (index >= 0)
    return vector_iget( context->run_args , index );
  else
    return NULL;
}




