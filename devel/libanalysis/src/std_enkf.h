#ifndef __SIMPLE_ENKF_H__
#define __SIMPLE_ENKF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <matrix.h>

#define  DEFAULT_ENKF_TRUNCATION_  0.99
#define  ENKF_TRUNCATION_KEY_      "ENKF_TRUNCATION"
#define  ENKF_NCOMP_KEY_           "ENKF_NCOMP" 

typedef struct std_enkf_data_struct std_enkf_data_type;


  void     std_enkf_initX(void * module_data , matrix_type * X , matrix_type * S , matrix_type * R , matrix_type * innov , matrix_type * E , matrix_type *D , matrix_type * randrot);
bool     std_enkf_set_double( void * arg , const char * var_name , double value);
bool     std_enkf_set_int( void * arg , const char * var_name , int value);

double   std_enkf_get_truncation( void * module_data );
void   * std_enkf_data_alloc( );
void     std_enkf_data_free( void * module_data );
int      std_enkf_get_subspace_dimension( void * module_data );



#ifdef __cplusplus
}
#endif

#endif





