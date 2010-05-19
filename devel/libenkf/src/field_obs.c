/**
   See the overview documentation of the observation system in enkf_obs.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util.h>
#include <stringlist.h>
#include <enkf_util.h>
#include <field_obs.h> 
#include <field_config.h>
#include <obs_data.h>
#include <meas_vector.h>
#include <field_config.h>
#include <field.h>
#include <active_list.h>
#include <stringlist.h>

#define FIELD_OBS_TYPE_ID 661098


struct field_obs_struct {
  UTIL_TYPE_ID_DECLARATION;
  char   * field_name;   		  /** The state keyword for the observed field - PRESSURE / SWAT / PORO /...   */
  char   * obs_label;    		  /** A user provided label for the observation.      */
  int      size;         		  /** The number of field cells observed.             */
  int    * index_list;   		  /** The list indices which are observed - (active indices). */
  int    * i;            		  /** The vector of indices i,j,k are equivalent to those in index_list - they are only retained for RFT plotting. */
  int    * j;
  int    * k;
  double * obs_value;    		  /** The observed values.                            */
  double * obs_std;      		  /** The standard deviation of the observations.     */
  stringlist_type * keylist;
  
  const field_config_type * field_config; /* The config object of the field we are observing - shared reference. */
};


static UTIL_SAFE_CAST_FUNCTION_CONST(field_obs , FIELD_OBS_TYPE_ID);
static UTIL_SAFE_CAST_FUNCTION(field_obs , FIELD_OBS_TYPE_ID);
UTIL_IS_INSTANCE_FUNCTION(field_obs , FIELD_OBS_TYPE_ID);



/**
   The input vectors i,j,k should contain offset zero values.
*/
field_obs_type * field_obs_alloc(
  const char   * obs_label,
  const field_config_type * field_config , 
  const char   * field_name,
  int            size,
  const int    * i,
  const int    * j,
  const int    * k,
  const double * obs_value,
  const double * obs_std)
{
  
  field_obs_type * field_obs = util_malloc(sizeof * field_obs, __func__);
  UTIL_TYPE_ID_INIT( field_obs , FIELD_OBS_TYPE_ID );
  field_obs->size            = size;
  field_obs->field_name      = util_alloc_string_copy(field_name);
  field_obs->obs_label       = util_alloc_string_copy(obs_label);
  field_obs->index_list      = util_malloc( size * sizeof * field_obs->index_list , __func__);
  field_obs->field_config    = field_config;
  field_obs->keylist         = stringlist_alloc_new();
  {
    int l;
    for (l = 0; l < size; l++) {
      if (field_config_ijk_valid(field_config , i[l] , j[l] , k[l])) {
	int active_index = field_config_active_index(field_config , i[l] , j[l] , k[l]);
	if (active_index >= 0) {
	  field_obs->index_list[l] = active_index;
          stringlist_append_owned_ref( field_obs->keylist , util_alloc_sprintf("%s:%d,%d,%d" , obs_label , i[l]+1 , j[l]+1 , k[l]+1));
        } else
	  util_abort("%s: sorry: cell:(%d,%d,%d) is not active - can not observe it. \n",__func__ , i[l]+1 , j[l]+1 , k[l]+1);
      } else
	util_abort("%s: sorry: cell (%d,%d,%d) is outside valid range:  \n",__func__ , i[l]+1 , j[l]+1 , k[l]+1);
    }
  }
  field_obs->i               = util_alloc_copy(i         , size * sizeof * i                    , __func__);
  field_obs->j               = util_alloc_copy(j         , size * sizeof * j                    , __func__);
  field_obs->k               = util_alloc_copy(k         , size * sizeof * k                    , __func__);
  field_obs->obs_value       = util_alloc_copy(obs_value , size * sizeof * field_obs->obs_value , __func__);
  field_obs->obs_std         = util_alloc_copy(obs_std   , size * sizeof * field_obs->obs_value , __func__);
  
  return field_obs;
}



void field_obs_free(
  field_obs_type * field_obs)
{
  stringlist_free( field_obs->keylist );
  free(field_obs->index_list);
  free(field_obs->obs_value);
  free(field_obs->obs_std);
  free(field_obs->field_name);
  free(field_obs->obs_label);
  free(field_obs->i);
  free(field_obs->j);
  free(field_obs->k);
  free(field_obs);
}






const char * field_obs_get_field_name(
  const field_obs_type * field_obs)
{
  return field_obs->field_name;
}




void field_obs_get_observations(const field_obs_type * field_obs,  int  restart_nr,  obs_data_type * obs_data,  const active_list_type * __active_list) {
  int i;
  active_mode_type active_mode = active_list_get_mode( __active_list );
  if (active_mode == ALL_ACTIVE) {
    for (i=0; i < field_obs->size; i++) 
      obs_data_add(obs_data , field_obs->obs_value[i] , field_obs->obs_std[i] , stringlist_iget(field_obs->keylist , i) );
    
  } else if (active_mode == PARTLY_ACTIVE) {
    const int   * active_list    = active_list_get_active( __active_list ); 
    int active_size              = active_list_get_active_size( __active_list );
    for (i =0 ; i < active_size; i++) {
      int iobs = active_list[i];
      obs_data_add(obs_data , field_obs->obs_value[iobs] , field_obs->obs_std[iobs] , stringlist_iget(field_obs->keylist , iobs) );
    }
  }
}




void field_obs_measure(const field_obs_type * field_obs, const field_type * field_state, meas_vector_type * meas_vector , const active_list_type * __active_list) {
  int iobs;
  
  active_mode_type active_mode = active_list_get_mode( __active_list );
  if (active_mode == ALL_ACTIVE) {
    for (iobs=0; iobs < field_obs->size; iobs++) {
      double value = field_iget_double(field_state , field_obs->index_list[iobs]);
      meas_vector_add(meas_vector , value);
    }
  } else if (active_mode == PARTLY_ACTIVE) {
    const int   * active_list    = active_list_get_active( __active_list ); 
    int active_size              = active_list_get_active_size( __active_list );
    for (int i =0 ; i < active_size; i++) {
      iobs = active_list[i];
      double value = field_iget_double(field_state , field_obs->index_list[iobs]);
      meas_vector_add(meas_vector , value);
    }
  }
}





double field_obs_chi2(const field_obs_type * field_obs,  const field_type     * field_state) {
  double sum_chi2 = 0;
  for (int i=0; i < field_obs->size; i++) {
    double x = (field_iget_double(field_state , field_obs->index_list[i]) - field_obs->obs_value[i]) / field_obs->obs_std[i];
    sum_chi2 += x*x;
  }
  return sum_chi2;
}



//bool field_obs_fwrite(
//  const field_obs_type * field_obs,
//  FILE                 * stream) {
//  util_fwrite_string(field_obs->field_name, stream);
//  util_fwrite_string(field_obs->obs_label , stream);
//
//  util_fwrite_int   (field_obs->size      , stream);
//
//  util_fwrite       (field_obs->index_list , sizeof *field_obs->index_list , field_obs->size, stream, __func__);
//  util_fwrite       (field_obs->obs_value  , sizeof *field_obs->obs_value  , field_obs->size, stream, __func__);
//  util_fwrite       (field_obs->obs_std    , sizeof *field_obs->obs_value  , field_obs->size, stream, __func__);
//
//  return true;
//}
//
//
//
//void field_obs_fread(
//  field_obs_type * field_obs,
//  FILE           * stream)
//{
//  field_obs->field_name = util_fread_alloc_string(stream);
//  field_obs->obs_label  = util_fread_alloc_string(stream);
//
//  field_obs->size       = util_fread_int(         stream);
//
//  util_fread       (field_obs->index_list , sizeof *field_obs->index_list , field_obs->size, stream, __func__);
//  util_fread       (field_obs->obs_value  , sizeof *field_obs->obs_value  , field_obs->size, stream, __func__);
//  util_fread       (field_obs->obs_std    , sizeof *field_obs->obs_value  , field_obs->size, stream, __func__);
//}


/**
   The index is into the the number of active cells which are observed by this observation.
*/
void field_obs_iget(const field_obs_type * field_obs, int index , double *value , double * std) {
  *value = field_obs->obs_value[index];
  *std   = field_obs->obs_std[index];
}


void field_obs_user_get(const field_obs_type * field_obs , const char * index_key , double *value , double * std, bool * valid) {
  int      i,j,k;
  int      parse_user_key = field_config_parse_user_key(field_obs->field_config , index_key , &i, &j , &k);

  *valid = false;
  if (parse_user_key == 0) {
    int active_index = field_config_active_index(field_obs->field_config , i,j,k);
    int l = 0;
    /* iterating through all the cells the observation is observing. */
    while (!(*valid) && l < field_obs->size) {
      if (field_obs->index_list[l] == active_index) {
	*value = field_obs->obs_value[l];
	*std   = field_obs->obs_std[l];
	*valid = true;
      }
      l++;
    }
    //if (!(*valid))
    // fprintf(stderr,": observation object does not observe index:%d,%d,%d \n",i+1,j+1,k+1);
  } else {
    /*
      if (parse_user_key == 1)
      fprintf(stderr,"Failed to parse \"%s\" as three integers \n",index_key);
    else if (parse_user_key == 2)
      fprintf(stderr," ijk: %d , %d, %d is invalid \n",i+1 , j + 1 , k + 1);
    else if (parse_user_key == 3)
      fprintf(stderr," ijk: %d , %d, %d is an inactive cell. \n",i+1 , j + 1 , k + 1);
    else
      util_abort("%s: internal error -invalid value:%d \n",__func__ , parse_user_key);
    */
  }
}




const int * field_obs_get_i(const field_obs_type * field_obs) {
  return field_obs->i;
}

const int * field_obs_get_j(const field_obs_type * field_obs) {
  return field_obs->j;
}

const int * field_obs_get_k(const field_obs_type * field_obs) {
  return field_obs->k;
}


/*
  Returns by reference i,j,k for observation point nr block_nr.
*/

void field_obs_iget_ijk(const field_obs_type * field_obs , int block_nr , int * i , int * j , int * k) {
  *i = field_obs->i[block_nr];
  *j = field_obs->j[block_nr];
  *k = field_obs->k[block_nr];
}


int field_obs_get_size(const field_obs_type * field_obs) {
  return field_obs->size;
}


/*****************************************************************/

VOID_FREE(field_obs)
VOID_GET_OBS(field_obs)
VOID_MEASURE(field_obs , field)
VOID_USER_GET_OBS(field_obs)
VOID_CHI2(field_obs , field)
