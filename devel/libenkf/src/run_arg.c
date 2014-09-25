/*
   Copyright (C) 2014  Statoil ASA, Norway. 
    
   The file 'run_arg.c' is part of ERT - Ensemble based Reservoir Tool. 
    
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


#include <ert/util/path_fmt.h>
#include <ert/util/subst_list.h>
#include <ert/util/type_macros.h>

#include <ert/enkf/enkf_types.h>
#include <ert/enkf/run_arg.h>


#define RUN_ARG_TYPE_ID 66143287

UTIL_SAFE_CAST_FUNCTION( run_arg , RUN_ARG_TYPE_ID )
UTIL_IS_INSTANCE_FUNCTION( run_arg , RUN_ARG_TYPE_ID )

void run_arg_set_run_path(run_arg_type * run_arg , int iens , path_fmt_type * run_path_fmt, const subst_list_type * state_subst_list) {
  util_safe_free(run_arg->run_path);
  {
    char * tmp1 = path_fmt_alloc_path(run_path_fmt , false , iens, run_arg->iter);    /* 1: Replace first %d with iens, if a second %d replace with iter */
    char * tmp2 = subst_list_alloc_filtered_string( state_subst_list , tmp1 );        /* 2: Filter out various magic strings like <CASE> and <CWD>. */
    run_arg->run_path = util_alloc_abs_path( tmp2 );                                  /* 3: Ensure that the path is absolute. */
    free( tmp1 );
    free( tmp2 );
  }
}



run_arg_type * run_arg_alloc(int iens , 
                             run_mode_type run_mode          , 
                             int init_step_parameters        ,      
                             state_enum init_state_parameter ,
                             state_enum init_state_dynamic   ,
                             int step1                       , 
                             int step2                       ,
                             int iter                        ,
                             const char * runpath) {
  
  run_arg_type * run_arg = util_malloc(sizeof * run_arg );
  UTIL_TYPE_ID_INIT(run_arg , RUN_ARG_TYPE_ID);
  
  run_arg->iens = iens;
  run_arg->run_mode = run_mode;
  run_arg->init_step_parameters = init_step_parameters;
  run_arg->init_state_parameter = init_state_parameter;
  run_arg->init_state_dynamic = init_state_dynamic;
  run_arg->step1 = step1;
  run_arg->step2 = step2;
  run_arg->iter = iter;
  run_arg->run_path = util_alloc_abs_path( runpath );
  run_arg->num_internal_submit = 0;

  if (step1 == 0)
    run_arg->load_start = 1;
  else
    run_arg->load_start = step1;
  
  return run_arg;
}


run_arg_type * run_arg_alloc_ENSEMBLE_EXPERIMENT(int iens , int iter , const char * runpath) {
  return run_arg_alloc(iens , ENSEMBLE_EXPERIMENT , 0 , ANALYZED , ANALYZED , 0 , 0 , iter , runpath);
}


run_arg_type * run_arg_alloc_INIT_ONLY(int iens , int iter , const char * runpath) {
  return run_arg_alloc(iens , INIT_ONLY , 0 , ANALYZED , ANALYZED , 0 , 0 , iter , runpath);
}



void run_arg_free(run_arg_type * run_arg) {
  util_safe_free(run_arg->run_path);
  free(run_arg);
}


void run_arg_free__(void * arg) {
  run_arg_type * run_arg = run_arg_safe_cast( arg );
  run_arg_free( run_arg );
}


void run_arg_complete_run(run_arg_type * run_arg) {
  if (run_arg->run_status == JOB_RUN_OK) {
    util_safe_free(run_arg->run_path);
    run_arg->run_path = NULL;
  }
}



void run_arg_increase_submit_count( run_arg_type * run_arg ) {
  run_arg->num_internal_submit++;
}



const char * run_arg_get_runpath( const run_arg_type * run_arg) {
  return run_arg->run_path;
}


run_status_type run_arg_get_run_status( const run_arg_type * run_arg ) {
  return run_arg->run_status;
}


int run_arg_get_iter( const run_arg_type * run_arg ) {
  return run_arg->iter;
}


int run_arg_get_iens( const run_arg_type * run_arg ) {
  return run_arg->iens;
}


void run_arg_set_inactive( run_arg_type * run_arg ) {
  run_arg->active = false;
}


int run_arg_get_queue_index( const run_arg_type * run_arg ) {
  return run_arg->queue_index;
}
