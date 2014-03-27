/*
   Copyright (C) 2013  Statoil ASA, Norway. 
    
   The file 'analysis_iter_config.c' is part of ERT - Ensemble based Reservoir Tool. 
    
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


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <ert/util/util.h>
#include <ert/util/stringlist.h>

#include <ert/config/config.h>

#include <ert/enkf/enkf_defaults.h>
#include <ert/enkf/config_keys.h>
#include <ert/enkf/analysis_iter_config.h>


struct analysis_iter_config_struct {
  char            * case_fmt;
  stringlist_type * storage;
  int               num_iterations;
  int               max_num_iterations;
  bool              case_set;   
  bool              num_iterations_set; 
  bool              max_num_iterations_set;
};


void analysis_iter_config_set_num_iterations( analysis_iter_config_type * config , int num_iterations) {
  config->num_iterations = num_iterations; 
  config->num_iterations_set = true;
}

int analysis_iter_config_get_num_iterations( const analysis_iter_config_type * config ) {
  return config->num_iterations;
}

bool analysis_iter_config_num_iterations_set( const analysis_iter_config_type * config ) {
  return config->num_iterations_set;
}


void analysis_iter_config_set_max_num_iterations( analysis_iter_config_type * config , int max_num_iterations) {
  config->max_num_iterations = max_num_iterations;
  config->max_num_iterations_set = true;
}

int analysis_iter_config_get_max_num_iterations( const analysis_iter_config_type * config ) {
  if (analysis_iter_config_max_num_iterations_set(config))
    return config->max_num_iterations;
  else
    return (2 * analysis_iter_config_get_num_iterations(config));
}

bool analysis_iter_config_max_num_iterations_set( const analysis_iter_config_type * config ) {
  return config->max_num_iterations_set;
}


void analysis_iter_config_set_case_fmt( analysis_iter_config_type * config , const char * case_fmt) {
  config->case_fmt = util_realloc_string_copy( config->case_fmt , case_fmt );
  config->case_set = true;
}


bool analysis_iter_config_case_fmt_set( const analysis_iter_config_type * config ) {
  return config->case_set;
}


char * analysis_iter_config_get_case_fmt( analysis_iter_config_type * config) {
  return config->case_fmt;
}


analysis_iter_config_type * analysis_iter_config_alloc() {
   analysis_iter_config_type * config = util_malloc( sizeof * config );  
   config->case_fmt = NULL;
   analysis_iter_config_set_case_fmt( config, DEFAULT_ANALYSIS_ITER_CASE);
   config->storage = stringlist_alloc_new();
   analysis_iter_config_set_num_iterations( config , DEFAULT_ANALYSIS_NUM_ITERATIONS );

   config->num_iterations_set = false;
   config->max_num_iterations_set = false;
   config->case_set = false;
   return config;
}

void analysis_iter_config_free( analysis_iter_config_type * config ) {
  util_safe_free( config->case_fmt );
  stringlist_free( config->storage );
  util_safe_free( config );
}


const char * analysis_iter_config_iget_case( analysis_iter_config_type * config , int iter) {
  if (config->case_fmt != NULL) {
    char * fs_case = util_alloc_sprintf( config->case_fmt , iter );
    stringlist_append_owned_ref( config->storage , fs_case);
    return fs_case;
  } else
    return NULL;
}


void analysis_iter_config_add_config_items( config_type * config ) {
  config_add_key_value( config , ITER_CASE_KEY        , false , CONFIG_STRING);
  config_add_key_value( config , ITER_COUNT_KEY       , false , CONFIG_INT);
  config_add_key_value( config , MAX_ITER_COUNT_KEY   , false , CONFIG_INT);
}


void analysis_iter_config_init(analysis_iter_config_type * iter_config , const config_type * config) {
  if (config_item_set( config , ITER_CASE_KEY ))
    analysis_iter_config_set_case_fmt( iter_config , config_get_value( config , ITER_CASE_KEY ));
  
  if (config_item_set( config , ITER_COUNT_KEY ))
    analysis_iter_config_set_num_iterations( iter_config , config_get_value_as_int( config , ITER_COUNT_KEY ));

  if (config_item_set( config , MAX_ITER_COUNT_KEY ))
    analysis_iter_config_set_max_num_iterations( iter_config , config_get_value_as_int( config , MAX_ITER_COUNT_KEY ));
}


