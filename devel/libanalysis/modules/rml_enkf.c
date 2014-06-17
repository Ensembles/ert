/*
   Copyright (C) 2011  Statoil ASA, Norway. 
    
   The file 'rml_enkf.c' is part of ERT - Ensemble based Reservoir Tool. 
    
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


#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <ert/util/util.h>
#include <ert/util/type_macros.h>
#include <ert/util/rng.h>
#include <ert/util/matrix.h>
#include <ert/util/matrix_blas.h>
#include <ert/util/bool_vector.h>

#include <ert/analysis/analysis_module.h>
#include <ert/analysis/analysis_table.h>
#include <ert/analysis/enkf_linalg.h>
#include <ert/analysis/std_enkf.h>

#include <rml_enkf_common.h>

typedef struct rml_enkf_data_struct rml_enkf_data_type;



//**********************************************
// DEFAULT PARAMS
//**********************************************
/*
  Observe that only one of the settings subspace_dimension and
  truncation can be valid at a time; otherwise the svd routine will
  fail. This implies that the set_truncation() and
  set_subspace_dimension() routines will set one variable, AND
  INVALIDATE THE OTHER. For most situations this will be OK, but if
  you have repeated calls to both of these functions the end result
  might be a surprise.  
*/
#define INVALID_SUBSPACE_DIMENSION     -1
#define INVALID_TRUNCATION             -1
#define DEFAULT_SUBSPACE_DIMENSION     INVALID_SUBSPACE_DIMENSION
#define DEFAULT_USE_PRIOR              true
#define DEFAULT_LAMBDA_INCREASE_FACTOR 4
#define DEFAULT_LAMBDA_REDUCE_FACTOR   0.1
#define DEFAULT_LAMBDA0                -1
#define DEFAULT_LAMBDA_MIN             0.01
#define DEFAULT_LAMBDA_RECALCULATE     false
#define DEFAULT_LOG_FILE               "rml_enkf.out"
#define DEFAULT_CLEAR_LOG              true

 

#define  USE_PRIOR_KEY               "USE_PRIOR"
#define  LAMBDA_REDUCE_FACTOR_KEY    "LAMBDA_REDUCE"
#define  LAMBDA_INCREASE_FACTOR_KEY  "LAMBDA_INCREASE"
#define  LAMBDA0_KEY                 "LAMBDA0"
#define  LAMBDA_MIN_KEY              "LAMBDA_MIN"
#define  LAMBDA_RECALCULATE_KEY      "LAMBDA_RECALCULATE"
#define  ITER_KEY                    "ITER"
#define  LOG_FILE_KEY                "LOG_FILE"
#define  CLEAR_LOG_KEY               "CLEAR_LOG" 




#define RML_ENKF_TYPE_ID 261123


//**********************************************
// RML "object" data definition
//**********************************************
/*
  The configuration data used by the rml_enkf module is contained in a
  rml_enkf_data_struct instance. The data type used for the rml_enkf
  module is quite simple; with only a few scalar variables, but there
  are essentially no limits to what you can pack into such a datatype.

  All the functions in the module have a void pointer as the first
  argument, this will immediately be casted to a rml_enkf_data_type
  instance, to get some type safety the UTIL_TYPE_ID system should be
  used (see documentation in util.h)

  The data structure holding the data for your analysis module should
  be created and initialized by a constructor, which should be
  registered with the '.alloc' element of the analysis table; in the
  same manner the desctruction of this data should be handled by a
  destructor or free() function registered with the .freef field of
  the analysis table.
*/
struct rml_enkf_data_struct {
  UTIL_TYPE_ID_DECLARATION;
  double    truncation;            // Controlled by config key: ENKF_TRUNCATION_KEY
  int       subspace_dimension;    // Controlled by config key: ENKF_NCOMP_KEY (-1: use Truncation instead)
  long      option_flags;
  int       iteration_nr;          // Keep track of the outer iteration loop
  double    Sk;                    // Objective function value
  double    Std;                   // Standard Deviation of the Objective function
  double  * Csc;
  matrix_type *Am;
  matrix_type *active_prior;
  matrix_type *prior0;
  matrix_type *state;
  bool_vector_type * ens_mask;
  bool use_prior;

  double    lambda;                 // parameter to control the setp length in Marquardt levenberg optimization 
  double    lambda0;
  double    lambda_min;
  double    lambda_reduce_factor;
  double    lambda_increase_factor;
  bool      lambda_recalculate;
  
  bool      clear_log;
  char    * log_file;
  FILE    * log_stream;
};



static UTIL_SAFE_CAST_FUNCTION( rml_enkf_data , RML_ENKF_TYPE_ID )
static UTIL_SAFE_CAST_FUNCTION_CONST( rml_enkf_data , RML_ENKF_TYPE_ID )






//**********************************************
// Set / Get
//**********************************************
double rml_enkf_get_truncation( rml_enkf_data_type * data ) {
  return data->truncation;
}

int rml_enkf_get_subspace_dimension( rml_enkf_data_type * data ) {
  return data->subspace_dimension;
}

void rml_enkf_set_truncation( rml_enkf_data_type * data , double truncation ) {
  data->truncation = truncation;
  if (truncation > 0.0)
    data->subspace_dimension = INVALID_SUBSPACE_DIMENSION;
}

void rml_enkf_set_lambda0( rml_enkf_data_type * data , double lambda0) {
  data->lambda0 = lambda0;
}

double rml_enkf_get_lambda0( const rml_enkf_data_type * data ) {
  return data->lambda0;
}

void rml_enkf_set_lambda_min( rml_enkf_data_type * data , double lambda_min) {
  data->lambda_min = lambda_min;
}

double rml_enkf_get_lambda_min( const rml_enkf_data_type * data ) {
  return data->lambda_min;
}

void rml_enkf_set_lambda_increase_factor( rml_enkf_data_type * data , double increase_factor) {
  data->lambda_increase_factor = increase_factor;
}

double rml_enkf_get_lambda_increase_factor( const rml_enkf_data_type * data ) {
  return data->lambda_increase_factor;
}

void rml_enkf_set_lambda_reduce_factor( rml_enkf_data_type * data , double reduce_factor) {
  data->lambda_reduce_factor = reduce_factor;
}

double rml_enkf_get_lambda_reduce_factor( const rml_enkf_data_type * data ) {
  return data->lambda_reduce_factor;
}

bool rml_enkf_get_use_prior( const rml_enkf_data_type * data ) {
  return data->use_prior;
}

void rml_enkf_set_use_prior( rml_enkf_data_type * data , bool use_prior) {
  data->use_prior = use_prior;
}

void rml_enkf_set_lambda_recalculate( rml_enkf_data_type * data , bool lambda_recalculate) {
  data->lambda_recalculate = lambda_recalculate;
}

void rml_enkf_set_subspace_dimension( rml_enkf_data_type * data , int subspace_dimension) {
  data->subspace_dimension = subspace_dimension;
  if (subspace_dimension > 0)
    data->truncation = INVALID_TRUNCATION;
}

void rml_enkf_set_iteration_nr( rml_enkf_data_type * data , int iteration_nr) {
  data->iteration_nr = iteration_nr;
}

int rml_enkf_get_iteration_nr( const rml_enkf_data_type * data ) {
  return data->iteration_nr;
}





//**********************************************
// Log-file related stuff
//**********************************************
bool rml_enkf_get_clear_log( const rml_enkf_data_type * data ) {
  return data->clear_log;
}

void rml_enkf_set_clear_log( rml_enkf_data_type * data , bool clear_log) {
  data->clear_log = clear_log;
}

void rml_enkf_set_log_file( rml_enkf_data_type * data , const char * log_file ) {
  data->log_file = util_realloc_string_copy( data->log_file , log_file );
}

const char * rml_enkf_get_log_file( const rml_enkf_data_type * data) {
  return data->log_file;
}

void rml_enkf_log_line( rml_enkf_data_type * data , const char * fmt , ...) {
  if (data->log_stream) {
    va_list ap;
    va_start(ap , fmt);
    vfprintf( data->log_stream , fmt , ap );
    va_end( ap );
  }
}

static void rml_enkf_write_log_header( rml_enkf_data_type * data, const char * format) {
  if (data->log_stream) {
    const char * column1 = "Iter#";
    const char * column2 = "Lambda";
    const char * column3 = "Sk old";
    const char * column4 = "Sk_new";
    const char * column5 = "std(Sk)";

         rml_enkf_log_line(data, format, column1, column2, column3, column4, column5);
  }
}

static void rml_enkf_write_iter_info( rml_enkf_data_type * data , double Sk_new, double Std_new ) {
  if (data->log_stream) {

         const char * format =         "\n%2d-->%-2d %-7.3f %-7.3f --> %-7.3f %-7.3f";
         const char * format_headers = "\n%7s %-7s %-7s --> %-7s %-7s";

         static int has_printed_header = 0;
         if (!has_printed_header) {
                 rml_enkf_write_log_header( data, format_headers );
                 has_printed_header = 1;
         }

         rml_enkf_log_line( data , format, data->iteration_nr, data->iteration_nr+1,  data->lambda, data->Sk, Sk_new, Std_new);
  }
}

static void rml_enkf_open_log_file( rml_enkf_data_type * data ) {
  data->log_stream = NULL;
  if (data->log_file) {
    if ( data->iteration_nr == 0) {
      if (data->clear_log){
        data->log_stream = util_mkdir_fopen( data->log_file , "w");
      }
      else
        data->log_stream = util_mkdir_fopen( data->log_file , "a");
    } else
      data->log_stream = util_fopen( data->log_file , "a");
  }
}




//**********************************************
// Memory
//**********************************************
void * rml_enkf_data_alloc( rng_type * rng) {
  rml_enkf_data_type * data = util_malloc( sizeof * data);
  UTIL_TYPE_ID_INIT( data , RML_ENKF_TYPE_ID );
    
  data->log_file     = NULL;

  rml_enkf_set_truncation( data , DEFAULT_ENKF_TRUNCATION_ );
  rml_enkf_set_subspace_dimension( data , DEFAULT_SUBSPACE_DIMENSION );
  rml_enkf_set_use_prior( data , DEFAULT_USE_PRIOR );
  rml_enkf_set_lambda0( data , DEFAULT_LAMBDA0 );
  rml_enkf_set_lambda_increase_factor(data , DEFAULT_LAMBDA_INCREASE_FACTOR);
  rml_enkf_set_lambda_reduce_factor(data , DEFAULT_LAMBDA_REDUCE_FACTOR);
  rml_enkf_set_lambda_min( data , DEFAULT_LAMBDA_MIN );
  rml_enkf_set_log_file( data , DEFAULT_LOG_FILE );
  rml_enkf_set_clear_log( data , DEFAULT_CLEAR_LOG );
  rml_enkf_set_lambda_recalculate( data , DEFAULT_LAMBDA_RECALCULATE );

  data->option_flags = ANALYSIS_NEED_ED + ANALYSIS_UPDATE_A + ANALYSIS_ITERABLE + ANALYSIS_SCALE_DATA;
  data->iteration_nr = 0;
  data->Std          = 0; 
  data->ens_mask     = bool_vector_alloc(0,false);
  data->state        = matrix_alloc(1,1);
  data->active_prior = matrix_alloc(1,1);
  data->prior0       = matrix_alloc(1,1);
  return data;
}

void rml_enkf_data_free( void * arg ) { 
  rml_enkf_data_type * data = rml_enkf_data_safe_cast( arg );

  matrix_free( data->state );
  matrix_free( data->prior0 );
  matrix_free( data->active_prior );

  util_safe_free( data->log_file );
  bool_vector_free( data->ens_mask );
  free( data );
}




//**********************************************
// Actual Algorithm, called through updateA()
//**********************************************
static void rml_enkf_init1__( rml_enkf_data_type * data) {
  

  int state_size    = matrix_get_rows( data->active_prior );
  int ens_size      = matrix_get_columns( data->active_prior );
  int nrmin         = util_int_min( ens_size , state_size); 
  matrix_type * Dm  = matrix_alloc_copy( data->active_prior );
  matrix_type * Um  = matrix_alloc( state_size , nrmin  );     /* Left singular vectors.  */
  matrix_type * VmT = matrix_alloc( nrmin , ens_size );        /* Right singular vectors. */
  double * Wm       = util_calloc( nrmin , sizeof * Wm ); 
  double nsc        = 1/sqrt(ens_size - 1); 

  matrix_subtract_row_mean(Dm);


  //This routine only computes the SVD of Ensemble State matrix  

  for (int i=0; i < state_size; i++){
    double sc = nsc / (data->Csc[i]);
    matrix_scale_row( Dm , i , sc);
  }
  int nsign1 = enkf_linalg_svd_truncation(Dm , data->truncation , -1 , DGESVD_MIN_RETURN  , Wm , Um , VmT);
  
  enkf_linalg_rml_enkfAm(Um, Wm, nsign1);

  data->Am = matrix_alloc_copy( Um );
  matrix_free(Um);
  matrix_free(VmT);
  matrix_free(Dm);
  free(Wm);
}

void rml_enkf_init_Csc(rml_enkf_data_type * data){
  int state_size = matrix_get_rows( data->active_prior );
  int ens_size   = matrix_get_columns( data->active_prior );

  for (int row=0; row < state_size; row++) {
    double sumrow = matrix_get_row_sum(data->active_prior , row);
    double tmp    = sumrow / ens_size;

    if (abs(tmp)< 1)
      data->Csc[row] = 0.05;
    else
      data->Csc[row] = 1.00;

  }
}

static void rml_enkf_initA__(rml_enkf_data_type * data, matrix_type * A, matrix_type * S, matrix_type * Cd, matrix_type * E, matrix_type * D, matrix_type * Udr, double * Wdr, matrix_type * VdTr) {

  int ens_size      = matrix_get_columns( S );
  double nsc        = 1/sqrt(ens_size-1);
  int nsign;
  {
    int nrobs         = matrix_get_rows( S );
    matrix_type *tmp  = matrix_alloc (nrobs, ens_size);
    matrix_subtract_row_mean( S );   
    matrix_inplace_diag_sqrt(Cd);
    matrix_matmul(tmp , Cd , S );
    matrix_scale(tmp , nsc);
  
    // SVD(S)  = Ud * Wd * Vd(T)
    nsign = enkf_linalg_svd_truncation(tmp , data->truncation , -1 , DGESVD_MIN_RETURN  , Wdr , Udr , VdTr);
    matrix_free( tmp );
  }
  
  {
    matrix_type * X3  = matrix_alloc( ens_size, ens_size );
    {
      matrix_type * X1  = matrix_alloc( nsign, ens_size);
      matrix_type * X2  = matrix_alloc( nsign, ens_size );
      
      
      enkf_linalg_rml_enkfX1(X1, Udr ,D ,Cd );                         // X1 = Ud(T)*Cd(-1/2)*D   -- D= -(dk-d0)
      enkf_linalg_rml_enkfX2(X2, Wdr ,X1 ,data->lambda + 1 , nsign);   // X2 = ((a*Ipd)+Wd^2)^-1  * X1
      enkf_linalg_rml_enkfX3(X3, VdTr ,Wdr,X2, nsign);                 // X3 = Vd *Wd*X2
      
      matrix_free(X2);
      matrix_free(X1);
    }
    
    {
      matrix_type * dA1 = matrix_alloc( matrix_get_rows(A) , ens_size);
      matrix_type * Dm = matrix_alloc_copy( A );

      matrix_subtract_row_mean( Dm );           /* Remove the mean from the ensemble of model parameters*/
      matrix_scale(Dm, nsc);

      matrix_matmul(dA1, Dm , X3);
      matrix_inplace_add(A,dA1);                // dA 

      matrix_free(Dm);
      matrix_free(dA1);
    }
    matrix_free(X3);

  }
}

void rml_enkf_init2__( rml_enkf_data_type * data, matrix_type *A, matrix_type *Acopy, double * Wdr, matrix_type * VdTr) {


  int state_size   = matrix_get_rows( Acopy );
  int ens_size     = matrix_get_columns( Acopy );
  double nsc       = 1/sqrt(ens_size-1); 

  matrix_type *Am  = matrix_alloc_copy(data->Am);
  matrix_type *Apr = matrix_alloc_copy(data->active_prior);


  int nsign1 = matrix_get_columns(data->Am);
  

  matrix_type * X4  = matrix_alloc(nsign1,ens_size);
  matrix_type * X5  = matrix_alloc(state_size,ens_size);
  matrix_type * X6  = matrix_alloc(ens_size,ens_size);
  matrix_type * X7  = matrix_alloc(ens_size,ens_size);
  matrix_type * dA2 = matrix_alloc(state_size , ens_size);
  matrix_type * Dk1 = matrix_alloc_copy( Acopy );
  
  {
    matrix_type * Dk = matrix_alloc_copy( Acopy );
    matrix_inplace_sub(Dk, Apr);
    rml_enkf_common_scaleA(Dk , data->Csc , true);
    matrix_dgemm(X4 , Am , Dk , true, false, 1.0, 0.0);
    matrix_free(Dk);
  }
  matrix_matmul(X5 , Am , X4);
  
  matrix_subtract_row_mean(Dk1);
  rml_enkf_common_scaleA(Dk1 , data->Csc , true);
  matrix_scale(Dk1,nsc);

  matrix_dgemm(X6, Dk1, X5, true, false, 1.0, 0.0);
  enkf_linalg_rml_enkfX7(X7, VdTr , Wdr , data->lambda + 1, X6);
  
  rml_enkf_common_scaleA(Dk1 , data->Csc , false);
  matrix_matmul(dA2 , Dk1 , X7);
  matrix_inplace_sub(A, dA2);

  matrix_free(Am);
  matrix_free(Apr);
  matrix_free(X4); 
  matrix_free(X5);
  matrix_free(X6);
  matrix_free(X7);
  matrix_free(dA2);
  matrix_free(Dk1);
}

static void rml_enkf_updateA_iter0(rml_enkf_data_type * data, matrix_type * A, matrix_type * S, matrix_type * R, matrix_type * dObs, matrix_type * E, matrix_type * D, matrix_type * Cd) {
        
  matrix_type * Skm = matrix_alloc(matrix_get_columns(D),matrix_get_columns(D));
  int ens_size      = matrix_get_columns( S );
  int nrobs         = matrix_get_rows( S );
  int nrmin         = util_int_min( ens_size , nrobs); 
  int state_size    = matrix_get_rows( A );
  matrix_type * Ud  = matrix_alloc( nrobs , nrmin    );    /* Left singular vectors.  */
  matrix_type * VdT = matrix_alloc( nrmin , ens_size );    /* Right singular vectors. */
  double * Wd       = util_calloc( nrmin , sizeof * Wd ); 

  data->Csc = util_calloc(state_size , sizeof * data->Csc);
  data->Sk  = enkf_linalg_data_mismatch(D,Cd,Skm);  
  data->Std = matrix_diag_std(Skm,data->Sk);
  
  if (data->lambda0 < 0)
    data->lambda = pow(10 , floor(log10(data->Sk/(2*nrobs))) );
  else
    data->lambda = data->lambda0;
  
  rml_enkf_common_store_state( data->state  , A , data->ens_mask );
  rml_enkf_common_store_state( data->prior0 , A , data->ens_mask );
  rml_enkf_common_recover_state( data->prior0 , data->active_prior , data->ens_mask );

  rml_enkf_initA__(data , A, S , Cd , E , D , Ud , Wd , VdT);
  if (data->use_prior) {
    rml_enkf_init_Csc( data );
    rml_enkf_init1__(data );
  }

  rml_enkf_write_iter_info(data, data->Sk, data->Std);

  matrix_free( Skm );
  matrix_free( Ud );
  matrix_free( VdT );
  free( Wd );
}

void rml_enkf_updateA(void * module_data, matrix_type * A, matrix_type * S, matrix_type * R, matrix_type * dObs, matrix_type * E, matrix_type * D) {


  rml_enkf_data_type * data = rml_enkf_data_safe_cast( module_data );
  double Sk_new;
  double  Std_new;
  int nrobs         = matrix_get_rows( S );
  int ens_size      = matrix_get_columns( S );
  double nsc        = 1/sqrt(ens_size-1); 
  matrix_type * Cd  = matrix_alloc( nrobs, nrobs );
 
  
  enkf_linalg_Covariance(Cd ,E ,nsc, nrobs);
  matrix_inv(Cd);

  rml_enkf_open_log_file(data);


  if (data->iteration_nr == 0) {
    rml_enkf_updateA_iter0(data , A , S , R , dObs , E , D , Cd);
    data->iteration_nr++;
  } else {
    int nrmin         = util_int_min( ens_size , nrobs); 
    matrix_type * Ud  = matrix_alloc( nrobs , nrmin    );    /* Left singular vectors.  */
    matrix_type * VdT = matrix_alloc( nrmin , ens_size );    /* Right singular vectors. */
    double * Wd       = util_calloc( nrmin , sizeof * Wd ); 
    matrix_type * Skm = matrix_alloc(matrix_get_columns(D),matrix_get_columns(D));
    matrix_type * Acopy  = matrix_alloc_copy (A);
    Sk_new = enkf_linalg_data_mismatch(D,Cd,Skm);  //Calculate the intitial data mismatch term
    Std_new = matrix_diag_std(Skm,Sk_new);
    
    if (data->lambda_recalculate)
      data->lambda = pow(10 , floor(log10(Sk_new / (2*nrobs))) );
    
    rml_enkf_common_recover_state( data->prior0 , data->active_prior , data->ens_mask );

    {
      bool mismatch_reduced = false;
      bool std_reduced = false;

      if (Sk_new < data->Sk)
        mismatch_reduced = true;
      
      if (Std_new <= data->Std)
        std_reduced = true;

			rml_enkf_write_iter_info(data, Sk_new, Std_new);

      if (mismatch_reduced) {
        /*
          Stop check: if ( (1- (Sk_new/data->Sk)) < .0001)  // check convergence ** model change norm has to be added in this!!
        */
        if (std_reduced) 
          data->lambda = data->lambda * data->lambda_reduce_factor;

        rml_enkf_common_store_state(data->state , A , data->ens_mask );

        data->Sk = Sk_new;
        data->Std=Std_new;
        data->iteration_nr++;
      } else {
        data->lambda = data->lambda * data->lambda_increase_factor;
        rml_enkf_common_recover_state( data->state , A , data->ens_mask );
      }
    }

    rml_enkf_initA__(data , A , S , Cd , E , D , Ud , Wd , VdT);
    if (data->use_prior) {
      rml_enkf_init_Csc( data );
      rml_enkf_init2__(data , A , Acopy , Wd , VdT);
    }
    matrix_free(Acopy);
    matrix_free(Skm);
    matrix_free( Ud );
    matrix_free( VdT );
    free( Wd );
  }

  if (data->lambda < data->lambda_min)
    data->lambda = data->lambda_min;


  if (data->log_stream)
    fclose( data->log_stream );
                 
  matrix_free(Cd);
}

void rml_enkf_init_update(void * arg, const bool_vector_type * ens_mask, const matrix_type * S, const matrix_type * R, const matrix_type * dObs, const matrix_type * E, const matrix_type * D ) {
  
  rml_enkf_data_type * module_data = rml_enkf_data_safe_cast( arg );
  bool_vector_memcpy( module_data->ens_mask , ens_mask );
}






//**********************************************
// Set / Get basic types
//**********************************************
bool rml_enkf_set_int( void * arg , const char * var_name , int value) {
  rml_enkf_data_type * module_data = rml_enkf_data_safe_cast( arg );
  {
    bool name_recognized = true;
    
    if (strcmp( var_name , ENKF_NCOMP_KEY_) == 0)
      rml_enkf_set_subspace_dimension( module_data , value );
    else if (strcmp( var_name , ITER_KEY) == 0)
      rml_enkf_set_iteration_nr( module_data , value );
    else
      name_recognized = false;

    return name_recognized;
  }
}

int rml_enkf_get_int( const void * arg, const char * var_name) {
  const rml_enkf_data_type * module_data = rml_enkf_data_safe_cast_const( arg );
  {
    if (strcmp(var_name , ITER_KEY) == 0)
      return module_data->iteration_nr;
    else
      return -1;
  }
}

bool rml_enkf_set_bool( void * arg , const char * var_name , bool value) {
  rml_enkf_data_type * module_data = rml_enkf_data_safe_cast( arg );
  {
    bool name_recognized = true;
    
    if (strcmp( var_name , USE_PRIOR_KEY) == 0)
      rml_enkf_set_use_prior( module_data , value );
    else if (strcmp( var_name , CLEAR_LOG_KEY) == 0)
      rml_enkf_set_clear_log( module_data , value );
    else if (strcmp( var_name , LAMBDA_RECALCULATE_KEY) == 0)
      rml_enkf_set_lambda_recalculate( module_data , value );
    else
      name_recognized = false;

    return name_recognized;
  }
}

bool rml_enkf_get_bool( const void * arg, const char * var_name) {
  const rml_enkf_data_type * module_data = rml_enkf_data_safe_cast_const( arg );
  {
    if (strcmp(var_name , USE_PRIOR_KEY) == 0)
      return module_data->use_prior;
    else if (strcmp(var_name , CLEAR_LOG_KEY) == 0) 
      return module_data->clear_log;
    else if (strcmp(var_name , LAMBDA_RECALCULATE_KEY) == 0) 
      return module_data->lambda_recalculate;
    else
       return false;
  }
}

bool rml_enkf_set_double( void * arg , const char * var_name , double value) {
  rml_enkf_data_type * module_data = rml_enkf_data_safe_cast( arg );
  {
    bool name_recognized = true;

    if (strcmp( var_name , ENKF_TRUNCATION_KEY_) == 0)
      rml_enkf_set_truncation( module_data , value );
    else if (strcmp( var_name , LAMBDA_INCREASE_FACTOR_KEY) == 0)
      rml_enkf_set_lambda_increase_factor( module_data , value );
    else if (strcmp( var_name , LAMBDA_REDUCE_FACTOR_KEY) == 0)
      rml_enkf_set_lambda_reduce_factor( module_data , value );
    else if (strcmp( var_name , LAMBDA0_KEY) == 0)
      rml_enkf_set_lambda0( module_data , value );
    else if (strcmp( var_name , LAMBDA_MIN_KEY) == 0)
      rml_enkf_set_lambda_min( module_data , value );
    else
      name_recognized = false;

    return name_recognized;
  }
}

double rml_enkf_get_double( const void * arg, const char * var_name) {
  const rml_enkf_data_type * module_data = rml_enkf_data_safe_cast_const( arg );
  {
    if (strcmp(var_name , LAMBDA_REDUCE_FACTOR_KEY) == 0)
      return module_data->lambda_reduce_factor;
    if (strcmp(var_name , LAMBDA_INCREASE_FACTOR_KEY) == 0)
      return module_data->lambda_increase_factor;
    if (strcmp(var_name , LAMBDA0_KEY) == 0)
      return module_data->lambda0;
    if (strcmp(var_name , LAMBDA_MIN_KEY) == 0)
      return module_data->lambda_min;
    if (strcmp(var_name , ENKF_TRUNCATION_KEY_) == 0)
      return module_data->truncation;
    else
      return -1;
  }
}


bool rml_enkf_set_string( void * arg , const char * var_name , const char * value) {
  rml_enkf_data_type * module_data = rml_enkf_data_safe_cast( arg );
  {
    bool name_recognized = true;
    
    if (strcmp( var_name , LOG_FILE_KEY) == 0)
      rml_enkf_set_log_file( module_data , value );
    else
      name_recognized = false;

    return name_recognized;
  }
}

long rml_enkf_get_options( void * arg , long flag ) {
  rml_enkf_data_type * module_data = rml_enkf_data_safe_cast( arg );
  {
    return module_data->option_flags;
  }
}

bool rml_enkf_has_var( const void * arg, const char * var_name) {
  {
    if (strcmp(var_name , ITER_KEY) == 0)
      return true;
    else if (strcmp(var_name , USE_PRIOR_KEY) == 0)
      return true;
    else if (strcmp(var_name , LAMBDA_INCREASE_FACTOR_KEY) == 0)
      return true;
    else if (strcmp(var_name , LAMBDA_REDUCE_FACTOR_KEY) == 0)
      return true;
    else if (strcmp(var_name , LAMBDA0_KEY) == 0)
      return true;
    else if (strcmp(var_name , LAMBDA_MIN_KEY) == 0)
      return true;
    else if (strcmp(var_name , LAMBDA_RECALCULATE_KEY) == 0)
      return true;
    else if (strcmp(var_name , ENKF_TRUNCATION_KEY_) == 0)
      return true;
    else if (strcmp(var_name , LOG_FILE_KEY) == 0)
      return true;
    else if (strcmp(var_name , CLEAR_LOG_KEY) == 0)
      return true;
    else
      return false;
  }
}

void * rml_enkf_get_ptr( const void * arg , const char * var_name ) {
  const rml_enkf_data_type * module_data = rml_enkf_data_safe_cast_const( arg );
  {
    if (strcmp(var_name , LOG_FILE_KEY) == 0)
      return module_data->log_file;
    else
      return NULL;
  }
}






//**********************************************
// Symbol table
//**********************************************
#ifdef INTERNAL_LINK
#define SYMBOL_TABLE rml_enkf_symbol_table
#else
#define SYMBOL_TABLE EXTERNAL_MODULE_SYMBOL
#endif


analysis_table_type SYMBOL_TABLE = {
  .alloc           = rml_enkf_data_alloc,
  .freef           = rml_enkf_data_free,
  .set_int         = rml_enkf_set_int , 
  .set_double      = rml_enkf_set_double , 
  .set_bool        = rml_enkf_set_bool, 
  .set_string      = rml_enkf_set_string,
  .get_options     = rml_enkf_get_options , 
  .initX           = NULL,
  .updateA         = rml_enkf_updateA ,  
  .init_update     = rml_enkf_init_update ,
  .complete_update = NULL,
  .has_var         = rml_enkf_has_var,
  .get_int         = rml_enkf_get_int,
  .get_double      = rml_enkf_get_double,
  .get_bool        = rml_enkf_get_bool,
  .get_ptr         = rml_enkf_get_ptr,
};

