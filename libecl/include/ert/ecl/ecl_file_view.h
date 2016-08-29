#ifndef ERT_ECL_FILE_VIEW_H
#define ERT_ECL_FILE_VIEW_H

#include <stdlib.h>
#include <stdbool.h>

#include <ert/ecl/ecl_kw.h>
#include <ert/ecl/ecl_file_kw.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  ECL_FILE_CLOSE_STREAM  =  1 ,  /*
                                    This flag will close the underlying FILE object between each access; this is
                                    mainly to save filedescriptors in cases where many ecl_file instances are open at
                                    the same time. */
  //
  ECL_FILE_WRITABLE      =  2    /*
                                    This flag opens the file in a mode where it can be updated and modified, but it
                                    must still exist and be readable. I.e. this should not compared with the normal:
                                    fopen(filename , "w") where an existing file is truncated to zero upon successfull
                                    open.
                                 */
} ecl_file_flag_type;


typedef struct ecl_file_view_struct ecl_file_view_type;

  bool ecl_file_view_flags_set( int state_flags , int query_flags);

  ecl_file_view_type * ecl_file_view_alloc( fortio_type * fortio , int * flags , inv_map_type * inv_map , bool owner );
  int ecl_file_view_get_global_index( const ecl_file_view_type * ecl_file_view , const char * kw , int ith);
  void ecl_file_view_make_index( ecl_file_view_type * ecl_file_view );
  bool ecl_file_view_has_kw( const ecl_file_view_type * ecl_file_view, const char * kw);
  ecl_file_kw_type * ecl_file_view_iget_file_kw( const ecl_file_view_type * ecl_file_view , int global_index);
  ecl_file_kw_type * ecl_file_view_iget_named_file_kw( const ecl_file_view_type * ecl_file_view , const char * kw, int ith);
  ecl_kw_type * ecl_file_view_iget_kw( const ecl_file_view_type * ecl_file_view , int index);
  void ecl_file_view_index_fload_kw(const ecl_file_view_type * ecl_file_view, const char* kw, int index, const int_vector_type * index_map, char* buffer);
  int ecl_file_view_find_kw_value( const ecl_file_view_type * ecl_file_view , const char * kw , const void * value);
  const char * ecl_file_view_iget_distinct_kw( const ecl_file_view_type * ecl_file_view , int index);
  int ecl_file_view_get_num_distinct_kw( const ecl_file_view_type * ecl_file_view );
  int ecl_file_view_get_size( const ecl_file_view_type * ecl_file_view );
  ecl_type_enum ecl_file_view_iget_type( const ecl_file_view_type * ecl_file_view , int index);
  int ecl_file_view_iget_size( const ecl_file_view_type * ecl_file_view , int index);
  const char * ecl_file_view_iget_header( const ecl_file_view_type * ecl_file_view , int index);
  ecl_kw_type * ecl_file_view_iget_named_kw( const ecl_file_view_type * ecl_file_view , const char * kw, int ith);
  ecl_type_enum ecl_file_view_iget_named_type( const ecl_file_view_type * ecl_file_view , const char * kw , int ith);
  int ecl_file_view_iget_named_size( const ecl_file_view_type * ecl_file_view , const char * kw , int ith);
  void ecl_file_view_replace_kw( ecl_file_view_type * ecl_file_view , ecl_kw_type * old_kw , ecl_kw_type * new_kw , bool insert_copy);
  bool ecl_file_view_load_all( ecl_file_view_type * ecl_file_view );
  void ecl_file_view_add_kw( ecl_file_view_type * ecl_file_view , ecl_file_kw_type * file_kw);
  void ecl_file_view_free( ecl_file_view_type * ecl_file_view );
  void ecl_file_view_free__( void * arg );
  int ecl_file_view_get_num_named_kw(const ecl_file_view_type * ecl_file_view , const char * kw);
  void ecl_file_view_fwrite( const ecl_file_view_type * ecl_file_view , fortio_type * target , int offset);
  int ecl_file_view_iget_occurence( const ecl_file_view_type * ecl_file_view , int global_index);
  void ecl_file_view_fprintf_kw_list(const ecl_file_view_type * ecl_file_view , FILE * stream);
  ecl_file_view_type * ecl_file_view_alloc_blockmap(const ecl_file_view_type * ecl_file_view , const char * header, int occurence);
  void ecl_file_view_add_child( ecl_file_view_type * parent , ecl_file_view_type * child);

  int ecl_file_view_seqnum_index_from_sim_time( ecl_file_view_type * parent_map , time_t sim_time);
  bool ecl_file_view_has_sim_time( const ecl_file_view_type * ecl_file_view , time_t sim_time);
  int ecl_file_view_find_sim_time(const ecl_file_view_type * ecl_file_view , time_t sim_time);
  double ecl_file_view_iget_restart_sim_days(const ecl_file_view_type * ecl_file_view , int seqnum_index);
  time_t ecl_file_view_iget_restart_sim_date(const ecl_file_view_type * ecl_file_view , int seqnum_index);
  bool ecl_file_view_has_report_step( const ecl_file_view_type * ecl_file_view , int report_step);

#ifdef __cplusplus
}
#endif

#endif
