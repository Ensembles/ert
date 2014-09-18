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

#ifndef __RUN_INFO_H__
#define __RUN_INFO_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <ert/util/path_fmt.h>
#include <ert/util/subst_list.h>


/**
   This struct is a pure utility structure used to pack the various
   bits and pieces of information needed to start, monitor, and load
   back results from the forward model simulations. 

   Typcially the values in this struct are set from the enkf_main
   object before a forward_step starts.
*/
 
typedef struct run_info_struct {
  bool                    __ready;              /* An attempt to check the internal state - not active yet. */
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
} run_info_type;
  

  

  void run_info_set_run_path(run_info_type * run_info , int iens , path_fmt_type * run_path_fmt, const subst_list_type * state_subst_list);

  run_info_type * run_info_alloc();
  void run_info_free(run_info_type * run_info);
  void run_info_complete_run(run_info_type * run_info);
  void run_info_init_for_load(run_info_type * run_info , 
                              int load_start, 
                              int step1,
                              int step2,
                              int iens,
                              int iter , 
                              path_fmt_type * run_path_fmt ,
                              const subst_list_type * state_subst_list);


  void run_info_init(run_info_type * run_info        , 
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
                     const subst_list_type * state_subst_list);



#ifdef __cplusplus
}
#endif
#endif
