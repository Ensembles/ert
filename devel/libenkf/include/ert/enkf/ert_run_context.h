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

#ifndef __ERT_RUN_CONTEXT_H__
#define __ERT_RUN_CONTEXT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <ert/util/type_macros.h>
#include <ert/util/bool_vector.h>

#include <ert/enkf/enkf_types.h>
  

typedef struct ert_run_context_struct ert_run_context_type;

  ert_run_context_type *   ert_run_context_alloc(bool_vector_type * iactive , 
                                                 run_mode_type run_mode , 
                                                 int load_start , 
                                                 int init_step_parameter , 
                                                 state_enum init_state_parameter,
                                                 state_enum init_state_dynamic , 
                                                 int iter , 
                                                 int step1 , 
                                                 int step2 );

  void                     ert_run_context_free( ert_run_context_type * );
  int                      ert_run_context_get_size( const ert_run_context_type * context );
  run_mode_type            ert_run_context_get_mode( const ert_run_context_type * context );
  const bool_vector_type * ert_run_context_get_iactive( const ert_run_context_type * context );
  int                      ert_run_context_get_step1( const ert_run_context_type * context );
  int                      ert_run_context_get_step2( const ert_run_context_type * context );

  UTIL_IS_INSTANCE_HEADER( ert_run_context );
  

#ifdef __cplusplus
}
#endif
#endif


