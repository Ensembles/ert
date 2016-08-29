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


typedef struct file_map_struct file_map_type;

  bool ecl_file_view_flags_set( int state_flags , int query_flags);

  file_map_type * file_map_alloc( fortio_type * fortio , int * flags , inv_map_type * inv_map , bool owner );
  int file_map_get_global_index( const file_map_type * file_map , const char * kw , int ith);
  void file_map_make_index( file_map_type * file_map );
  bool file_map_has_kw( const file_map_type * file_map, const char * kw);
  ecl_file_kw_type * file_map_iget_file_kw( const file_map_type * file_map , int global_index);
  ecl_file_kw_type * file_map_iget_named_file_kw( const file_map_type * file_map , const char * kw, int ith);
  ecl_kw_type * file_map_iget_kw( const file_map_type * file_map , int index);
  void file_map_index_fload_kw(const file_map_type * file_map, const char* kw, int index, const int_vector_type * index_map, char* buffer);
  int file_map_find_kw_value( const file_map_type * file_map , const char * kw , const void * value);
  const char * file_map_iget_distinct_kw( const file_map_type * file_map , int index);
  int file_map_get_num_distinct_kw( const file_map_type * file_map );
  int file_map_get_size( const file_map_type * file_map );
  ecl_type_enum file_map_iget_type( const file_map_type * file_map , int index);
  int file_map_iget_size( const file_map_type * file_map , int index);
  const char * file_map_iget_header( const file_map_type * file_map , int index);
  ecl_kw_type * file_map_iget_named_kw( const file_map_type * file_map , const char * kw, int ith);
  ecl_type_enum file_map_iget_named_type( const file_map_type * file_map , const char * kw , int ith);
  int file_map_iget_named_size( const file_map_type * file_map , const char * kw , int ith);
  void file_map_replace_kw( file_map_type * file_map , ecl_kw_type * old_kw , ecl_kw_type * new_kw , bool insert_copy);
  bool file_map_load_all( file_map_type * file_map );
  void file_map_add_kw( file_map_type * file_map , ecl_file_kw_type * file_kw);
  void file_map_free( file_map_type * file_map );
  void file_map_free__( void * arg );
  int file_map_get_num_named_kw(const file_map_type * file_map , const char * kw);
  void file_map_fwrite( const file_map_type * file_map , fortio_type * target , int offset);
  int file_map_iget_occurence( const file_map_type * file_map , int global_index);
  void file_map_fprintf_kw_list(const file_map_type * file_map , FILE * stream);
  file_map_type * file_map_alloc_blockmap(const file_map_type * file_map , const char * header, int occurence);

  int file_map_seqnum_index_from_sim_time( file_map_type * parent_map , time_t sim_time);
  bool file_map_has_sim_time( const file_map_type * file_map , time_t sim_time);
  int file_map_find_sim_time(const file_map_type * file_map , time_t sim_time);
  double file_map_iget_restart_sim_days(const file_map_type * file_map , int seqnum_index);
  time_t file_map_iget_restart_sim_date(const file_map_type * file_map , int seqnum_index);
  bool file_map_has_report_step( const file_map_type * file_map , int report_step);

#ifdef __cplusplus
}
#endif

#endif
