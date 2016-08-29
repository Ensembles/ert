/*
   Copyright (C) 2016  Statoil ASA, Norway.

   The file 'ecl_file_view.c' is part of ERT - Ensemble based Reservoir Tool.

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


#include <ert/util/vector.h>
#include <ert/util/hash.h>
#include <ert/util/stringlist.h>

#include <ert/ecl/fortio.h>
#include <ert/ecl/ecl_kw.h>
#include <ert/ecl/ecl_kw_magic.h>
#include <ert/ecl/ecl_file_kw.h>
#include <ert/ecl/ecl_file_view.h>
#include <ert/ecl/ecl_rsthead.h>


struct file_map_struct {
  vector_type       * kw_list;      /* This is a vector of ecl_file_kw instances corresponding to the content of the file. */
  hash_type         * kw_index;     /* A hash table with integer vectors of indices - see comment below. */
  stringlist_type   * distinct_kw;  /* A stringlist of the keywords occuring in the file - each string occurs ONLY ONCE. */
  fortio_type       * fortio;       /* The same fortio instance pointer as in the ecl_file styructure. */
  bool                owner;        /* Is this map the owner of the ecl_file_kw instances; only true for the global_map. */
  inv_map_type     *  inv_map;       /* Shared reference owned by the ecl_file structure. */
  int               * flags;
};



/*****************************************************************/
/* Here comes the functions related to the index file_map. These
   functions are all of them static.
*/


bool ecl_file_view_flags_set( int state_flags , int query_flags) {
  if ((state_flags & query_flags) == query_flags)
    return true;
  else
    return false;
}



file_map_type * file_map_alloc( fortio_type * fortio , int * flags , inv_map_type * inv_map , bool owner ) {
  file_map_type * file_map     = util_malloc( sizeof * file_map );
  file_map->kw_list            = vector_alloc_new();
  file_map->kw_index           = hash_alloc();
  file_map->distinct_kw        = stringlist_alloc_new();
  file_map->owner              = owner;
  file_map->fortio             = fortio;
  file_map->inv_map            = inv_map;
  file_map->flags          = flags;
  return file_map;
}

int file_map_get_global_index( const file_map_type * file_map , const char * kw , int ith) {
  const int_vector_type * index_vector = hash_get(file_map->kw_index , kw);
  int global_index = int_vector_iget( index_vector , ith);
  return global_index;
}



/**
   This function iterates over the kw_list vector and builds the
   internal index fields 'kw_index' and 'distinct_kw'. This function
   must be called every time the content of the kw_list vector is
   modified (otherwise the ecl_file instance will be in an
   inconsistent state).
*/


void file_map_make_index( file_map_type * file_map ) {
  stringlist_clear( file_map->distinct_kw );
  hash_clear( file_map->kw_index );
  {
    int i;
    for (i=0; i < vector_get_size( file_map->kw_list ); i++) {
      const ecl_file_kw_type * file_kw = vector_iget_const( file_map->kw_list , i);
      const char             * header  = ecl_file_kw_get_header( file_kw );
      if ( !hash_has_key( file_map->kw_index , header )) {
        int_vector_type * index_vector = int_vector_alloc( 0 , -1 );
        hash_insert_hash_owned_ref( file_map->kw_index , header , index_vector , int_vector_free__);
        stringlist_append_copy( file_map->distinct_kw , header);
      }

      {
        int_vector_type * index_vector = hash_get( file_map->kw_index , header);
        int_vector_append( index_vector , i);
      }
    }
  }
}

bool file_map_has_kw( const file_map_type * file_map, const char * kw) {
  return hash_has_key( file_map->kw_index , kw );
}


ecl_file_kw_type * file_map_iget_file_kw( const file_map_type * file_map , int global_index) {
  ecl_file_kw_type * file_kw = vector_iget( file_map->kw_list , global_index);
  return file_kw;
}

ecl_file_kw_type * file_map_iget_named_file_kw( const file_map_type * file_map , const char * kw, int ith) {
  int global_index = file_map_get_global_index( file_map , kw , ith);
  ecl_file_kw_type * file_kw = file_map_iget_file_kw( file_map , global_index );
  return file_kw;
}



ecl_kw_type * file_map_iget_kw( const file_map_type * file_map , int index) {
  ecl_file_kw_type * file_kw = file_map_iget_file_kw( file_map , index );
  ecl_kw_type * ecl_kw = ecl_file_kw_get_kw_ptr( file_kw , file_map->fortio , file_map->inv_map);
  if (!ecl_kw) {
    if (fortio_assert_stream_open( file_map->fortio )) {

      ecl_kw = ecl_file_kw_get_kw( file_kw , file_map->fortio , file_map->inv_map);

      if (ecl_file_view_flags_set( file_map->flags[0] , ECL_FILE_CLOSE_STREAM))
        fortio_fclose_stream( file_map->fortio );
    }
  }
  return ecl_kw;
}

void file_map_index_fload_kw(const file_map_type * file_map, const char* kw, int index, const int_vector_type * index_map, char* buffer) {
    ecl_file_kw_type * file_kw = file_map_iget_named_file_kw( file_map , kw , index);

    if (fortio_assert_stream_open( file_map->fortio )) {
        offset_type offset = ecl_file_kw_get_offset(file_kw);
        ecl_type_enum ecl_type = ecl_file_kw_get_type(file_kw);
        int element_count = ecl_file_kw_get_size(file_kw);

        ecl_kw_fread_indexed_data(file_map->fortio, offset + ECL_KW_HEADER_FORTIO_SIZE, ecl_type, element_count, index_map, buffer);
    }
}


int file_map_find_kw_value( const file_map_type * file_map , const char * kw , const void * value) {
  int global_index = -1;
  if ( file_map_has_kw( file_map , kw)) {
    const int_vector_type * index_list = hash_get( file_map->kw_index , kw );
    int index = 0;
    while (index < int_vector_size( index_list )) {
      const ecl_kw_type * ecl_kw = file_map_iget_kw( file_map , int_vector_iget( index_list , index ));
      if (ecl_kw_data_equal( ecl_kw , value )) {
        global_index = int_vector_iget( index_list , index );
        break;
      }
      index++;
    }
  }
  return global_index;
}

const char * file_map_iget_distinct_kw( const file_map_type * file_map , int index) {
  return stringlist_iget( file_map->distinct_kw , index);
}

int file_map_get_num_distinct_kw( const file_map_type * file_map ) {
  return stringlist_get_size( file_map->distinct_kw );
}

int file_map_get_size( const file_map_type * file_map ) {
  return vector_get_size( file_map->kw_list );
}


ecl_type_enum file_map_iget_type( const file_map_type * file_map , int index) {
  ecl_file_kw_type * file_kw = file_map_iget_file_kw( file_map , index );
  return ecl_file_kw_get_type( file_kw );
}

int file_map_iget_size( const file_map_type * file_map , int index) {
  ecl_file_kw_type * file_kw = file_map_iget_file_kw( file_map , index );
  return ecl_file_kw_get_size( file_kw );
}

const char * file_map_iget_header( const file_map_type * file_map , int index) {
  ecl_file_kw_type * file_kw = file_map_iget_file_kw( file_map , index );
  return ecl_file_kw_get_header( file_kw );
}


ecl_kw_type * file_map_iget_named_kw( const file_map_type * file_map , const char * kw, int ith) {
  ecl_file_kw_type * file_kw = file_map_iget_named_file_kw( file_map , kw , ith);
  ecl_kw_type * ecl_kw = ecl_file_kw_get_kw_ptr( file_kw , file_map->fortio , file_map->inv_map );
  if (!ecl_kw) {
    if (fortio_assert_stream_open( file_map->fortio )) {

      ecl_kw = ecl_file_kw_get_kw( file_kw , file_map->fortio , file_map->inv_map);

      if (ecl_file_view_flags_set( file_map->flags[0] , ECL_FILE_CLOSE_STREAM))
        fortio_fclose_stream( file_map->fortio );
    }
  }
  return ecl_kw;
}

ecl_type_enum file_map_iget_named_type( const file_map_type * file_map , const char * kw , int ith) {
  ecl_file_kw_type * file_kw = file_map_iget_named_file_kw( file_map , kw, ith);
  return ecl_file_kw_get_type( file_kw );
}

int file_map_iget_named_size( const file_map_type * file_map , const char * kw , int ith) {
  ecl_file_kw_type * file_kw = file_map_iget_named_file_kw( file_map , kw , ith );
  return ecl_file_kw_get_size( file_kw );
}


void file_map_replace_kw( file_map_type * file_map , ecl_kw_type * old_kw , ecl_kw_type * new_kw , bool insert_copy) {
  int index = 0;
  while (index < vector_get_size( file_map->kw_list )) {
    ecl_file_kw_type * ikw = vector_iget( file_map->kw_list , index );
    if (ecl_file_kw_ptr_eq( ikw , old_kw)) {
      /*
         Found it; observe that the vector_iset() function will
         automatically invoke the destructor on the old_kw.
      */
      ecl_kw_type * insert_kw = new_kw;

      if (insert_copy)
        insert_kw = ecl_kw_alloc_copy( new_kw );
      ecl_file_kw_replace_kw( ikw , file_map->fortio , insert_kw );

      file_map_make_index( file_map );
      return;
    }
    index++;
  }
  util_abort("%s: could not find ecl_kw ptr: %p \n",__func__ , old_kw);
}


bool file_map_load_all( file_map_type * file_map ) {
  bool loadOK = false;

  if (fortio_assert_stream_open( file_map->fortio )) {
    int index;
    for (index = 0; index < vector_get_size( file_map->kw_list); index++) {
      ecl_file_kw_type * ikw = vector_iget( file_map->kw_list , index );
      ecl_file_kw_get_kw( ikw , file_map->fortio , file_map->inv_map);
    }
    loadOK = true;
  }

  if (ecl_file_view_flags_set( file_map->flags[0] , ECL_FILE_CLOSE_STREAM))
    fortio_fclose_stream( file_map->fortio );

  return loadOK;
}


/*****************************************************************/



void file_map_add_kw( file_map_type * file_map , ecl_file_kw_type * file_kw) {
  if (file_map->owner)
    vector_append_owned_ref( file_map->kw_list , file_kw , ecl_file_kw_free__ );
  else
    vector_append_ref( file_map->kw_list , file_kw);
}

void file_map_free( file_map_type * file_map ) {
  hash_free( file_map->kw_index );
  stringlist_free( file_map->distinct_kw );
  vector_free( file_map->kw_list );
  free( file_map );
}

void file_map_free__( void * arg ) {
  file_map_type * file_map = ( file_map_type * ) arg;
  file_map_free( file_map );
}


int file_map_get_num_named_kw(const file_map_type * file_map , const char * kw) {
  if (hash_has_key(file_map->kw_index , kw)) {
    const int_vector_type * index_vector = hash_get(file_map->kw_index , kw);
    return int_vector_size( index_vector );
  } else
    return 0;
}

void file_map_fwrite( const file_map_type * file_map , fortio_type * target , int offset) {
  int index;
  for (index = offset; index < vector_get_size( file_map->kw_list ); index++) {
    ecl_kw_type * ecl_kw = file_map_iget_kw( file_map , index );
    ecl_kw_fwrite( ecl_kw , target );
  }
}




int file_map_iget_occurence( const file_map_type * file_map , int global_index) {
  const ecl_file_kw_type * file_kw = vector_iget_const( file_map->kw_list , global_index);
  const char * header              = ecl_file_kw_get_header( file_kw );
  const int_vector_type * index_vector = hash_get( file_map->kw_index , header );
  const int * index_data = int_vector_get_const_ptr( index_vector );

  int occurence = -1;
  {
    /* Manual reverse lookup. */
    int i;
    for (i=0; i < int_vector_size( index_vector ); i++)
      if (index_data[i] == global_index)
        occurence = i;
  }
  if (occurence < 0)
    util_abort("%s: internal error ... \n" , __func__);

  return occurence;
}

void file_map_fprintf_kw_list(const file_map_type * file_map , FILE * stream) {
  int i;
  for (i=0; i < vector_get_size( file_map->kw_list ); i++) {
    const ecl_file_kw_type * file_kw = vector_iget_const( file_map->kw_list , i );
    fprintf(stream , "%-8s %7d:%s\n",
            ecl_file_kw_get_header( file_kw ) ,
            ecl_file_kw_get_size( file_kw ) ,
            ecl_util_get_type_name( ecl_file_kw_get_type( file_kw )));
  }
}

/**
   Will return NULL if the block which is asked for is not present.
*/
file_map_type * file_map_alloc_blockmap(const file_map_type * file_map , const char * header, int occurence) {
  if (file_map_get_num_named_kw( file_map , header ) > occurence) {
    file_map_type * block_map = file_map_alloc( file_map->fortio , file_map->flags , file_map->inv_map , false);
    if (file_map_has_kw( file_map , header )) {
      int kw_index = file_map_get_global_index( file_map , header , occurence );
      ecl_file_kw_type * file_kw = vector_iget( file_map->kw_list , kw_index );

      while (true) {
        file_map_add_kw( block_map , file_kw );

        kw_index++;
        if (kw_index == vector_get_size( file_map->kw_list ))
          break;
        else {
          file_kw = vector_iget(file_map->kw_list , kw_index);
          if (strcmp( header , ecl_file_kw_get_header( file_kw )) == 0)
            break;
        }
      }
    }
    file_map_make_index( block_map );
    return block_map;
  } else
    return NULL;
}


/*****************************************************************/
/*                   R E S T A R T   F I L E S                   */
/*****************************************************************/



/*
   There is no special datastructure for working with restart files,
   they are 100% stock ecl_file instances with the following limited
   structure:

   * They are organized in blocks; where each block starts with a
     SEQNUM keyword, which contains the report step.

   * Each block contains an INTEHEAD keyword, immediately after the
     SEQNUM keyword, which contains the true simulation date of of the
     block, and also some other data. Observe that also INIT files and
     GRID files contain an INTEHEAD keyword; and that for files with
     LGRs there is one INTEHEAD keyword for each LGR. This creates an
     extra level of mess.

   The natural time ordering when working with the file data is just
   the running index in the file; however from a user perspective the
   natural way to indicate time coordinate is through the report step
   or the true simulation time (i.e. 22.th of October 2009). This file
   is all about converting the natural input unit time and report_step
   to the internal indexing. This is achieved by consulting the value
   of the INTEHEAD and SEQNUM keywords respectively.
*/

/*
About the time-direction
========================

For the following discussion we will focus on the following simplified
unified restart file. The leading number is the global index of the
keyword, the value in [] corresponds to the relevant part of the
content of the keyword on the line, the labels A,B,C,D,E are used for
references in the text below.

 0 | SEQNUM   [0]           \  A
 1 | INTEHEAD [01.01.2005]  |
 2 | PRESSURE [... ]        |
 3 | SWAT     [...]         |
   | -----------------------+
 4 | SEQNUM   [5]           |  B
 5 | INTEHEAD [01.06.2005]  |
 6 | PRESSURE [... ]        |
 7 | SWAT     [...]         |
   |------------------------+
 8 | SEQNUM   [10]          |  C
 9 | INTEHEAD [01.12.2006]  |
10 | PRESSURE [...]         |
11 | SWAT     [...]         |
   |------------------------+
12 | SEQNUM   [20]          |  D
13 | INTEHEAD [01.12.2007]  |
14 | PRESSURE [...]         |
15 | SWAT     [...]         |
16 | OIL_DEN  [...]         |
   |------------------------+
12 | SEQNUM   [40]          |  E
13 | INTEHEAD [01.12.2009]  |
14 | PRESSURE [...]         |
15 | SWAT     [...]         /


This restart file has the following features:

 o It contains in total 16 keywords.

 o It contains 5 blocks of collected keywords corresponding to one
   time instant, each of these blocks is called a report_step,
   typcially coming from one DATES keyword in the ECLIPSE
   datafile. Observe that the file does not have the block structure
   visualized on this figure, the only thing separating the blocks in
   the file is the occurence of a SEQNUM keyword.

 o Only a few of the report steps are present, namely 0, 5, 10, 20 and
   40.

 o The different blocks are not equally long, the fourth block has an
   extra keyword OIL_DEN.

To adress these keywords and blocks using different time coordinates
we have introduced the following concepts:

 report_step: This corresponds to the value of the SEQNUM keword,
    i.e. to do queries based on the report_step we must load the
    seqnum kewyord and read the value.

        ecl_file_get_unrstmap_report_step( ecl_file , 0 ) => A
        ecl_file_get_unrstmap_report_step( ecl_file , 1 ) => NULL

        ecl_file_has_report_step( ecl_file , 5 ) => True
        ecl_file_has_report_step( ecl_file , 2 ) => False

 sim_time: This correpsonds to the true simulation time of the report
    step, the simulation time is stored as integers DAY, MONTH, YEAR
    in the INTEHEAD keyword; the function INTEHEAD_date() will extract
    the DAY, MONTH and YEAR values from an INTEHEAD keyword instance
    and convert to a time_t instance. The functions:

     ecl_file_get_unrstmap_sim_time() and ecl_file_has_has_sim_time()

    can be used to query for simulation times and get the
    corresponding block maps.

 index/global_index : This is typically the global running index of
    the keyword in the file; this is the unique address of the keyword
    which is used for the final lookup.

 occurence: The nth' time a particular keyword has occured in the
    file, i.e. the SEQNUM keyword in block C is the third occurence of
    SEQNUM. Instead of occurence xxxx_index is also used to indicate
    the occurence of keyword xxxx. The occurence number is the integer
    argument to the xxx_iget_named_kw() function, and also the final
    call to create blockmaps.

*/


bool file_map_has_report_step( const file_map_type * file_map , int report_step) {
  int global_index = file_map_find_kw_value( file_map , SEQNUM_KW , &report_step );
  if (global_index >= 0)
    return true;
  else
    return false;
}


time_t file_map_iget_restart_sim_date(const file_map_type * file_map , int seqnum_index) {
  time_t sim_time = -1;
  file_map_type * seqnum_map = seqnum_map = file_map_alloc_blockmap( file_map , SEQNUM_KW , seqnum_index);

  if (seqnum_map != NULL) {
    ecl_kw_type * intehead_kw = file_map_iget_named_kw( seqnum_map , INTEHEAD_KW , 0);
    sim_time = ecl_rsthead_date( intehead_kw );
    file_map_free( seqnum_map );
  }

  return sim_time;
}


double file_map_iget_restart_sim_days(const file_map_type * file_map , int seqnum_index) {
  double sim_days = 0;
  file_map_type * seqnum_map = seqnum_map = file_map_alloc_blockmap( file_map , SEQNUM_KW , seqnum_index);

  if (seqnum_map != NULL) {
    ecl_kw_type * doubhead_kw = file_map_iget_named_kw( seqnum_map , DOUBHEAD_KW , 0);
    sim_days = ecl_kw_iget_double( doubhead_kw , DOUBHEAD_DAYS_INDEX);
    file_map_free( seqnum_map );
  }

  return sim_days;
}




int file_map_find_sim_time(const file_map_type * file_map , time_t sim_time) {
  int seqnum_index = -1;
  if ( file_map_has_kw( file_map , INTEHEAD_KW)) {
    const int_vector_type * intehead_index_list = hash_get( file_map->kw_index , INTEHEAD_KW );
    int index = 0;
    while (index < int_vector_size( intehead_index_list )) {
      const ecl_kw_type * intehead_kw = file_map_iget_kw( file_map , int_vector_iget( intehead_index_list , index ));
      if (ecl_rsthead_date( intehead_kw ) == sim_time) {
        seqnum_index = index;
        break;
      }
      index++;
    }
  }
  return seqnum_index;
}


/**
   This function will scan through the ecl_file looking for INTEHEAD
   headers corresponding to sim_time. If sim_time is found the
   function will return the INTEHEAD occurence number, i.e. for a
   unified restart file like:

   INTEHEAD  /  01.01.2000
   ...
   PRESSURE
   SWAT
   ...
   INTEHEAD  /  01.03.2000
   ...
   PRESSURE
   SWAT
   ...
   INTEHEAD  /  01.05.2000
   ...
   PRESSURE
   SWAT
   ....

   The function call:

   ecl_file_get_restart_index( restart_file , (time_t) "01.03.2000")

   will return 1. Observe that this will in general NOT agree with the
   DATES step number.

   If the sim_time can not be found the function will return -1, that
   includes the case when the file in question is not a restart file
   at all, and no INTEHEAD headers can be found.

   Observe that the function requires on-the-second-equality; which is
   of course quite strict.

   Each report step only has one occurence of SEQNUM, but one INTEHEAD
   for each LGR; i.e. one should call iselect_rstblock() prior to
   calling this function.
*/


bool file_map_has_sim_time( const file_map_type * file_map , time_t sim_time) {
  int num_INTEHEAD = file_map_get_num_named_kw( file_map , INTEHEAD_KW );
  if (num_INTEHEAD == 0)
    return false;       /* We have no INTEHEAD headers - probably not a restart file at all. */
  else {
    /*
      Should probably do something smarter than a linear search; but I dare not
      take the chance that all INTEHEAD headers are properly set. This is from
      Schlumberger after all.
    */
    int intehead_index = 0;
    while (true) {
      time_t itime = file_map_iget_restart_sim_date( file_map , intehead_index );

      if (itime == sim_time) /* Perfect hit. */
        return true;

      if (itime > sim_time)  /* We have gone past the target_time - i.e. we do not have it. */
        return false;

      intehead_index++;
      if (intehead_index == num_INTEHEAD)  /* We have iterated through the whole thing without finding sim_time. */
        return false;
    }
  }
}


int file_map_seqnum_index_from_sim_time( file_map_type * parent_map , time_t sim_time) {
  int num_seqnum = file_map_get_num_named_kw( parent_map , SEQNUM_KW );
  int seqnum_index = 0;
  file_map_type * seqnum_map;

  while (true) {
    seqnum_map = file_map_alloc_blockmap( parent_map , SEQNUM_KW , seqnum_index);

    if (seqnum_map != NULL) {
      if (file_map_has_sim_time( seqnum_map , sim_time)) {
        file_map_free( seqnum_map );
        return seqnum_index;
      } else {
        file_map_free( seqnum_map );
        seqnum_index++;
      }
    }

    if (num_seqnum == seqnum_index)
      return -1;
  }
}


