/*
   Copyright (C) 2012  Statoil ASA, Norway.

   The file 'ert_workflow_list.c' is part of ERT - Ensemble based Reservoir Tool.

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
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include <ert/util/hash.h>
#include <ert/util/set.h>
#include <ert/util/ert_version.h>
#include <ert/util/stringlist.h>
#include <ert/util/util.h>
#include <ert/util/subst_list.h>
#include <ert/util/type_macros.h>

#include <ert/config/config_parser.h>
#include <ert/config/config_error.h>
#include <ert/config/config_schema_item.h>

#include <ert/job_queue/workflow.h>
#include <ert/job_queue/workflow_job.h>
#include <ert/job_queue/workflow_joblist.h>

#include <ert/enkf/ert_workflow_list.h>
#include <ert/enkf/config_keys.h>
#include <ert/enkf/enkf_defaults.h>
#include <ert/enkf/ert_log.h>


#define ERT_WORKFLOW_LIST_TYPE_ID 8856275

struct ert_workflow_list_struct {
  UTIL_TYPE_ID_DECLARATION;
  stringlist_type         * path_list;
  hash_type               * workflows;
  hash_type               * alias_map;
  workflow_joblist_type   * joblist;
  const subst_list_type   * context;
  const config_error_type * last_error;
  bool                      verbose;
};



ert_workflow_list_type * ert_workflow_list_alloc(const subst_list_type * context) {
  ert_workflow_list_type * workflow_list = util_malloc( sizeof * workflow_list );
  UTIL_TYPE_ID_INIT( workflow_list , ERT_WORKFLOW_LIST_TYPE_ID );
  workflow_list->path_list  = stringlist_alloc_new();
  workflow_list->workflows  = hash_alloc();
  workflow_list->alias_map  = hash_alloc();
  workflow_list->joblist    = workflow_joblist_alloc();
  workflow_list->context    = context;
  workflow_list->last_error = NULL;
  ert_workflow_list_set_verbose( workflow_list , DEFAULT_WORKFLOW_VERBOSE );
  return workflow_list;
}



UTIL_IS_INSTANCE_FUNCTION( ert_workflow_list , ERT_WORKFLOW_LIST_TYPE_ID )

void ert_workflow_list_set_verbose( ert_workflow_list_type * workflow_list , bool verbose) {
  workflow_list->verbose = verbose;
}


const subst_list_type * ert_workflow_list_get_context(const ert_workflow_list_type * workflow_list) {
    return workflow_list->context;
}

void ert_workflow_list_free( ert_workflow_list_type * workflow_list ) {
  hash_free( workflow_list->workflows );
  hash_free( workflow_list->alias_map );
  stringlist_free( workflow_list->path_list );
  workflow_joblist_free( workflow_list->joblist );
  free( workflow_list );
}



workflow_type * ert_workflow_list_add_workflow( ert_workflow_list_type * workflow_list , const char * workflow_file , const char * workflow_name) {
  if (util_file_exists( workflow_file )) {
    workflow_type * workflow = workflow_alloc( workflow_file , workflow_list->joblist );
    char * name;

    if (workflow_name == NULL)
      util_alloc_file_components( workflow_file , NULL , &name , NULL );
    else
      name = (char *) workflow_name;


    hash_insert_hash_owned_ref( workflow_list->workflows , name , workflow , workflow_free__);
    if (hash_has_key( workflow_list->alias_map , name))
      hash_del( workflow_list->alias_map , name);

    if (workflow_name == NULL)
      free( name );

    return workflow;
  } else
    return NULL;
}



void ert_workflow_list_add_alias( ert_workflow_list_type * workflow_list , const char * real_name , const char * alias) {
  if (!util_string_equal( real_name , alias))
    hash_insert_ref( workflow_list->alias_map , alias , real_name );
}


void ert_workflow_list_add_job( ert_workflow_list_type * workflow_list , const char * job_name , const char * config_file ) {
  char * name = (char *) job_name;

  if (job_name == NULL)
    util_alloc_file_components( config_file , NULL , &name , NULL );

  if (!workflow_joblist_add_job_from_file( workflow_list->joblist , name , config_file ))
    fprintf(stderr,"** Warning: failed to add workflow job:%s from:%s \n",name , config_file );

  if (job_name == NULL)
    free(name);
}


bool ert_workflow_list_has_job( const ert_workflow_list_type * workflow_list , const char * job_name) {
  return workflow_joblist_has_job( workflow_list->joblist , job_name );
}


const workflow_job_type * ert_workflow_list_get_job( const ert_workflow_list_type * workflow_list , const char * job_name) {
    return workflow_joblist_get_job(workflow_list->joblist, job_name);
}

/**
   This function will create the most specific matching filename
   corresponding to the @root_name input. I.e. it will look for
   filename in this order:

     ROOT@X.Y.Z, ROOT@X.Y, ROOT@X, ROOT

   And return the first name corresponding to an existing file. If no
   file can be found the function will return NULL.
*/

static char * ert_workflow_list_alloc_version_name( const char * path , const char * root_name ) {
  char * full_path = util_alloc_sprintf( "%s%s%s@%d.%d.%s" , path , UTIL_PATH_SEP_STRING , root_name ,
					 version_get_major_ert_version(),
					 version_get_minor_ert_version(),
					 version_get_micro_ert_version());
  if (util_file_exists( full_path ))
    return full_path;
  else
    free( full_path );

  /*****************************************************************/

  full_path = util_alloc_sprintf( "%s%s%s@%d.%d" , path , UTIL_PATH_SEP_STRING , root_name ,
				  version_get_major_ert_version(),
				  version_get_minor_ert_version());

  if (util_file_exists( full_path ))
    return full_path;
  else
    free( full_path );

  /*****************************************************************/

  full_path = util_alloc_sprintf( "%s%s%s@%d" , path , UTIL_PATH_SEP_STRING , root_name ,
				  version_get_major_ert_version());

  if (util_file_exists( full_path ))
    return full_path;
  else
    free( full_path );


  /*****************************************************************/

  full_path = util_alloc_sprintf( "%s%s%s" , path , UTIL_PATH_SEP_STRING , root_name);

  if (util_file_exists( full_path ))
    return full_path;
  else
    free( full_path );

  return NULL;
}



void ert_workflow_list_add_jobs_in_directory( ert_workflow_list_type * workflow_list , const char * path ) {
  DIR * dirH = opendir( path );
  set_type * names = set_alloc( 0 , NULL );
  if (dirH) {
    while (true) {
      struct dirent * entry = readdir( dirH );
      if (entry != NULL) {
        if ((strcmp(entry->d_name , ".") != 0) && (strcmp(entry->d_name , "..") != 0)) {
	  char * root_name, * version;
	  util_binary_split_string( entry->d_name , "@" , false , &root_name , &version);
	  if (!set_has_key( names , root_name)) {
	    char * full_path = ert_workflow_list_alloc_version_name( path , root_name );

	    if (full_path) {
	      set_add_key( names , root_name );
	      if (ert_log_is_open())
		ert_log_add_message( 1 , NULL , util_alloc_sprintf("Adding workflow job:%s " , full_path ), true);

	      ert_workflow_list_add_job( workflow_list , root_name , full_path );
	    }

	    free( full_path );
	  }
	  free( root_name );
	  free( version );
        }
      } else
        break;
    }
    closedir( dirH );
  } else
    fprintf(stderr, "** Warning: failed to open workflow/jobs directory: %s\n", path);

  set_free( names );
}


stringlist_type * ert_workflow_list_get_job_names(const ert_workflow_list_type * workflow_list) {
    return  workflow_joblist_get_job_names(workflow_list->joblist);
}


void ert_workflow_list_init( ert_workflow_list_type * workflow_list , config_content_type * config ) {
  /* Adding jobs */
  {
    if (config_content_has_item( config , WORKFLOW_JOB_DIRECTORY_KEY)) {
      const config_content_item_type * jobpath_item = config_content_get_item( config , WORKFLOW_JOB_DIRECTORY_KEY);
      for (int i=0; i < config_content_item_get_size( jobpath_item ); i++) {
        config_content_node_type * path_node = config_content_item_iget_node( jobpath_item , i );

        for (int j=0; j < config_content_node_get_size( path_node ); j++)
          ert_workflow_list_add_jobs_in_directory( workflow_list , config_content_node_iget_as_abspath( path_node , j ) );
      }
    }
  }

  {
    if (config_content_has_item( config , LOAD_WORKFLOW_JOB_KEY)) {
      const config_content_item_type * job_item = config_content_get_item( config , LOAD_WORKFLOW_JOB_KEY);
      for (int i=0; i < config_content_item_get_size( job_item ); i++) {
        config_content_node_type * job_node = config_content_item_iget_node( job_item , i );
        const char * config_file = config_content_node_iget_as_path( job_node , 0 );
        const char * job_name = config_content_node_safe_iget( job_node , 1 );
        ert_workflow_list_add_job( workflow_list , job_name , config_file);
      }
    }
  }


  /* Adding workflows */
  {
    if (config_content_has_item( config , LOAD_WORKFLOW_KEY)) {
      const config_content_item_type * workflow_item = config_content_get_item( config , LOAD_WORKFLOW_KEY);
      for (int i=0; i < config_content_item_get_size( workflow_item ); i++) {
        config_content_node_type * workflow_node = config_content_item_iget_node( workflow_item , i );
        const char * workflow_file = config_content_node_iget_as_path( workflow_node , 0 );
        const char * workflow_name = config_content_node_safe_iget( workflow_node , 1 );

        ert_workflow_list_add_workflow( workflow_list , workflow_file , workflow_name );
      }
    }
  }
}


void ert_workflow_list_add_config_items( config_parser_type * config ) {
  config_schema_item_type * item = config_add_schema_item( config , WORKFLOW_JOB_DIRECTORY_KEY , false  );
  config_schema_item_set_argc_minmax(item , 1 , 1 );
  config_schema_item_iset_type( item , 0 , CONFIG_PATH );

  item = config_add_schema_item( config , LOAD_WORKFLOW_KEY , false  );
  config_schema_item_set_argc_minmax(item , 1 , 2 );
  config_schema_item_iset_type( item , 0 , CONFIG_EXISTING_PATH );

  item = config_add_schema_item( config , LOAD_WORKFLOW_JOB_KEY , false  );
  config_schema_item_set_argc_minmax(item , 1 , 2 );
  config_schema_item_iset_type( item , 0 , CONFIG_EXISTING_PATH );
}



workflow_type *  ert_workflow_list_get_workflow(ert_workflow_list_type * workflow_list , const char * workflow_name ) {
  const char * lookup_name = workflow_name;

  if (hash_has_key( workflow_list->alias_map , workflow_name))
    lookup_name = hash_get( workflow_list->alias_map , workflow_name );

  return hash_get( workflow_list->workflows , lookup_name );
}

bool  ert_workflow_list_has_workflow(ert_workflow_list_type * workflow_list , const char * workflow_name ) {
  return
    hash_has_key( workflow_list->workflows , workflow_name ) ||
    hash_has_key( workflow_list->alias_map , workflow_name);
}


bool ert_workflow_list_run_workflow__(ert_workflow_list_type * workflow_list, workflow_type * workflow, bool verbose , void * self) {
  bool runOK = workflow_run( workflow, self , verbose , workflow_list->context);
  if (runOK)
    workflow_list->last_error = NULL;
  else
    workflow_list->last_error = workflow_get_last_error( workflow );

  return runOK;
}


bool ert_workflow_list_run_workflow_blocking(ert_workflow_list_type * workflow_list  , const char * workflow_name , void * self) {
  workflow_type * workflow = ert_workflow_list_get_workflow( workflow_list , workflow_name );
  bool result = ert_workflow_list_run_workflow__( workflow_list, workflow , workflow_list->verbose , self);
  return result;
}


bool ert_workflow_list_run_workflow(ert_workflow_list_type * workflow_list, const char * workflow_name , void * self) {
  workflow_type * workflow = ert_workflow_list_get_workflow( workflow_list , workflow_name );
  return ert_workflow_list_run_workflow__( workflow_list, workflow , workflow_list->verbose , self);
}


/*****************************************************************/

stringlist_type * ert_workflow_list_alloc_namelist( ert_workflow_list_type * workflow_list ) {
  return hash_alloc_stringlist( workflow_list->workflows );
}


const config_error_type * ert_workflow_list_get_last_error( const ert_workflow_list_type * workflow_list) {
  return workflow_list->last_error;
}


int ert_workflow_list_get_size( const ert_workflow_list_type * workflow_list) {
  return hash_get_size( workflow_list->workflows ) + hash_get_size( workflow_list->alias_map);
}
