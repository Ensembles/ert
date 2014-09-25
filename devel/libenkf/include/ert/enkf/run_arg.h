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

#ifndef __RUN_ARG_H__
#define __RUN_ARG_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <ert/util/path_fmt.h>
#include <ert/util/subst_list.h>
#include <ert/util/type_macros.h>

#include <ert/enkf/enkf_types.h>

/**
   This struct is a pure utility structure used to pack the various
   bits and pieces of information needed to start, monitor, and load
   back results from the forward model simulations. 

   Typcially the values in this struct are set from the enkf_main
   object before a forward_step starts.
*/
  
typedef struct run_arg_struct {
  UTIL_TYPE_ID_DECLARATION;
  bool                    __ready;              /* An attempt to check the internal state - not really used. */
  int                     iens;
  bool                    active;               /* Is this state object active at all - used for instance in ensemble experiments where only some of the members are integrated. */
  int                     init_step_parameters; /* The report step we initialize parameters from - will often be equal to step1, but can be different. */
  state_enum              init_state_parameter; /* Whether we should init from a forecast or an analyzed state - parameters. */
  state_enum              init_state_dynamic;   /* Whether we should init from a forecast or an analyzed state - dynamic state variables. */
  int                     max_internal_submit;  /* How many times the enkf_state object should try to resubmit when the queueu has said everything is OK - but the load fails. */  
  int                     num_internal_submit;   
  int                     load_start;           /* When loading back results - start at this step. */
  int                     step1;                /* The forward model is integrated: step1 -> step2 */
  int                     step2;
  int                     iter;
  char                  * run_path;             /* The currently used  runpath - is realloced / freed for every step. */
  run_mode_type           run_mode;             /* What type of run this is */
  int                     queue_index;          /* The job will in general have a different index in the queue than the iens number. */
  /******************************************************************/
  /* Return value - set by the called routine!!  */
  run_status_type         run_status;
} run_arg_type;
  

UTIL_SAFE_CAST_HEADER( run_arg );
UTIL_IS_INSTANCE_HEADER( run_arg );  

  void run_arg_set_run_path(run_arg_type * run_arg , int iens , path_fmt_type * run_path_fmt, const subst_list_type * state_subst_list);

  run_arg_type * run_arg_alloc(int iens , 
                               run_mode_type run_mode          , 
                               int init_step_parameters        ,      
                               state_enum init_state_parameter ,
                               state_enum init_state_dynamic   ,
                               int step1                       , 
                               int step2                       ,
                               int iter                        ,
                               const char * runpath);
  
  
  run_arg_type * run_arg_alloc_ENSEMBLE_EXPERIMENT(int iens , int iter , const char * runpath);
  run_arg_type * run_arg_alloc_INIT_ONLY(int iens , int iter , const char * runpath);

  int            run_arg_get_iens( const run_arg_type * run_arg );
  int            run_arg_get_iter( const run_arg_type * run_arg );
  void           run_arg_increase_submit_count( run_arg_type * run_arg );
  void run_arg_free(run_arg_type * run_arg);
  void run_arg_free__(void * arg);
  const char * run_arg_get_runpath( const run_arg_type * run_arg);
  void run_arg_complete_run(run_arg_type * run_arg);
  run_status_type run_arg_get_run_status( const run_arg_type * run_arg );
  
  void run_arg_set_inactive( run_arg_type * run_arg );
  int  run_arg_get_queue_index( const run_arg_type * run_arg );

#ifdef __cplusplus
}
#endif
#endif
