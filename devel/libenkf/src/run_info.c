/*
   Copyright (C) 2014  Statoil ASA, Norway. 
    
   The file 'run_info.c' is part of ERT - Ensemble based Reservoir Tool. 
    
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

#include <ert/enkf/enkf_types.h>
#include <ert/enkf/run_info.h>

void run_info_set_run_path(run_info_type * run_info , int iens , path_fmt_type * run_path_fmt, const subst_list_type * state_subst_list) {
  util_safe_free(run_info->run_path);
  {
    char * tmp1 = path_fmt_alloc_path(run_path_fmt , false , iens, run_info->iter);   /* 1: Replace first %d with iens, if a second %d replace with iter */
    char * tmp2 = subst_list_alloc_filtered_string( state_subst_list , tmp1 );        /* 2: Filter out various magic strings like <CASE> and <CWD>. */
    run_info->run_path = util_alloc_abs_path( tmp2 );                                 /* 3: Ensure that the path is absolute. */
    free( tmp1 );
    free( tmp2 );
  }
}



/**
   This function sets the run_info parameters. This is typically called
   (via an enkf_state__ routine) by the external scope handling the forward model.

   When this initialization code has been run we are certain that the
   enkf_state object has all the information it needs to "run itself"
   forward.

   
   This function inits the necessary fields in the run_info structure
   to be able to use the xxx_internalize_xxx() functions. Observe that
   trying actually run after the run_info structure has only been
   initialized here will lead to hard failure.

   The inits performed are essential for running, not only for the
   internalizing.
*/


void run_info_init_for_load(run_info_type * run_info , 
                            int load_start, 
                            int step1,
                            int step2,
                            int iens,
                            int iter , 
                            path_fmt_type * run_path_fmt ,
                            const subst_list_type * state_subst_list) {
  run_info->step1      = step1;
  run_info->step2      = step2;
  run_info->load_start = load_start;
  run_info->iter       = iter;
  run_info_set_run_path(run_info , iens , run_path_fmt , state_subst_list );
}



void run_info_set(run_info_type * run_info        , 
                  run_mode_type run_mode          , 
                  bool active                     , 
                  int max_internal_submit         ,
                  int init_step_parameters        ,      
                  state_enum init_state_parameter ,
                  state_enum init_state_dynamic   ,
                  int load_start                  , 
                  int step1                       , 
                  int step2                       ,
                  int iter                        ,
                  int iens                             , 
                  path_fmt_type * run_path_fmt ,
                  const subst_list_type * state_subst_list) {
  
  run_info->active               = active;
  run_info->init_step_parameters = init_step_parameters;
  run_info->init_state_parameter = init_state_parameter;
  run_info->init_state_dynamic   = init_state_dynamic;
  run_info->run_status           = JOB_NOT_STARTED;
  run_info->__ready              = true;
  run_info->run_mode             = run_mode;
  run_info->max_internal_submit  = max_internal_submit;
  run_info->num_internal_submit  = 0;
  run_info_init_for_load( run_info , load_start , step1 , step2 , iens , iter , run_path_fmt , state_subst_list);
}


run_info_type * run_info_alloc() {
  run_info_type * run_info = util_malloc(sizeof * run_info );
  run_info->run_path = NULL;
  return run_info;
}


void run_info_free(run_info_type * run_info) {
  util_safe_free(run_info->run_path);
  free(run_info);
}


void run_info_complete_run(run_info_type * run_info) {
  if (run_info->run_status == JOB_RUN_OK) {
    util_safe_free(run_info->run_path);
    run_info->run_path = NULL;
  }
}
