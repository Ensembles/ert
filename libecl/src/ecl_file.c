/*
   Copyright (C) 2011  Statoil ASA, Norway.

   The file 'ecl_file.c' is part of ERT - Ensemble based Reservoir Tool.

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

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <time.h>

#include <ert/util/hash.h>
#include <ert/util/util.h>
#include <ert/util/vector.h>
#include <ert/util/int_vector.h>
#include <ert/util/stringlist.h>

#include <ert/ecl/fortio.h>
#include <ert/ecl/ecl_kw.h>
#include <ert/ecl/ecl_file.h>
#include <ert/ecl/ecl_file_view.h>
#include <ert/ecl/ecl_endian_flip.h>
#include <ert/ecl/ecl_kw_magic.h>
#include <ert/ecl/ecl_rsthead.h>
#include <ert/ecl/ecl_file_kw.h>

/**
   This file implements functionality to load an ECLIPSE file in
   ecl_kw format. The implementation works by first searching through
   the complete file to create an index over all the keywords present
   in the file. The actual keyword data is not loaded before they are
   explicitly requested.

   The ecl_file_type is the middle layer of abstraction in the libecl
   hierarchy (see the file overview.txt in this directory); it works
   with a collection of ecl_kw instances and has various query
   functions, however it does not utilize knowledge of the
   structure/content of the files in the way e.g. ecl_grid.c does[1].

   The main datatype here is the ecl_file type, but in addition each
   ecl_kw instance is wrapped in an ecl_file_kw (implemented in
   ecl_file_kw.c) structure and all the indexing is implemented with
   the ecl_file_view type. The ecl_file_view type is not used outside this file.

   When the file is opened an index of all the keywords is created and
   stored in the field global_map, and the field active_view is set to
   point to global_map, i.e. all query/get operations on the ecl_file
   will be based on the complete index:

   In many cases (in particular for unified restart files) it is quite
   painful to work with this large and unvieldy index, and it is
   convenient to create a sub index based on a subset of the
   keywords. The creation of these sub indices is based on identifying
   a keyword from name and occurence number, and then including all
   keywords up to the next occurence of the same keyword:

      SEQHDR            ---\
      MINISTEP  0          |
      PARAMS    .....      |
      MINISTEP  1          |   Block 0
      PARAMS    .....      |
      MINISTEP  2          |
      PARAMS    .....      |
      SEQHDR            ---+
      MINISTEP  3          |
      PARAMS    .....      |
      MINISTEP  4          |   Block 1
      PARAMS    .....      |
      MINISTEP  5          |
      SEQHDR            ---+
      MINISTEP  6          |   Block 2
      PARAMS    ....       |
      SEQHDR            ---+
      MINISTEP  7          |
      PARAMS    ....       |   Block 3
      MINISTEP  8          |
      PARAMS    ....       |

   For the unified summary file depicted here e.g. the call

      ecl_file_get_blockmap( ecl_file , "SEQHDR" , 2 )

   Will create a sub-index consisting of the (three) keywords in what
   is called 'Block 2' in the figure above. In particular for restart
   files this abstraction is very convenient, because an extra layer
   of functionality is required to get from natural time coordinates
   (i.e. simulation time or report step) to the occurence number (see
   ecl_rstfile for more details).

   To select a subindex as the active index you use the
   ecl_file_select_block() function, or alternatively you can use
   ecl_file_open_block() to directly select the relevant block
   immediately after the open() statement. Observe that when using a
   sub index thorugh ecl_file_select_block() function the global_map
   will still be present in the ecl_file instance, and subsequent
   calls to create a new sub index will also use the global_map index
   - i.e. the indexing is not recursive, a sub index is always created
   based on the global_map, and not on the currently active map.


   [1]: This is not entirely true - in the file ecl_rstfile.c; which
        is included from this file are several specialized function
        for working with restart files. However the restart files are
        still treated as collections of ecl_kw instances, and not
        internalized as in e.g. ecl_sum.
*/



#define ECL_FILE_ID 776107





struct ecl_file_struct {
  UTIL_TYPE_ID_DECLARATION;
  fortio_type       * fortio;       /* The source of all the keywords - must be retained
                                       open for reading for the entire lifetime of the
                                       ecl_file object. */
  ecl_file_view_type * global_view;       /* The index of all the ecl_kw instances in the file. */
  ecl_file_view_type * active_view;       /* The currently active index. */
  bool            read_only;
  int             flags;
  vector_type   * map_stack;
  inv_map_type  * inv_view;
};


/*
  This illustrates the indexing. The ecl_file instance contains in
  total 7 ecl_kw instances, the global index [0...6] is the internal
  way to access the various keywords. The kw_index is a hash table
  with entries 'SEQHDR', 'MINISTEP' and 'PARAMS'. Each entry in the
  hash table is an integer vector which again contains the internal
  index of the various occurences:

   ------------------
   SEQHDR            \
   MINISTEP  0        |
   PARAMS    .....    |
   MINISTEP  1        |
   PARAMS    .....    |
   MINISTEP  2        |
   PARAMS    .....   /
   ------------------

   kw_index    = {"SEQHDR": [0], "MINISTEP": [1,3,5], "PARAMS": [2,4,6]}    <== This is hash table.
   kw_list     = [SEQHDR , MINISTEP , PARAMS , MINISTEP , PARAMS , MINISTEP , PARAMS]
   distinct_kw = [SEQHDR , MINISTEP , PARAMS]

*/




/*****************************************************************/
/* Here comes the implementation of the ecl_file proper 'class'. */


UTIL_SAFE_CAST_FUNCTION( ecl_file , ECL_FILE_ID)
UTIL_IS_INSTANCE_FUNCTION( ecl_file , ECL_FILE_ID)



ecl_file_type * ecl_file_alloc_empty( int flags ) {
  ecl_file_type * ecl_file = util_malloc( sizeof * ecl_file );
  UTIL_TYPE_ID_INIT(ecl_file , ECL_FILE_ID);
  ecl_file->map_stack = vector_alloc_new();
  ecl_file->inv_view  = inv_map_alloc( );
  ecl_file->flags     = flags;
  return ecl_file;
}


/*****************************************************************/
/* fwrite functions */

void ecl_file_fwrite_fortio(const ecl_file_type * ecl_file , fortio_type * target, int offset) {
  ecl_file_view_fwrite( ecl_file->active_view , target , offset );
}



/*
   Observe : if the filename is a standard filename which can be used
   to infer formatted/unformatted automagically the fmt_file variable
   is NOT consulted.
*/

void ecl_file_fwrite(const ecl_file_type * ecl_file , const char * filename, bool fmt_file) {
  bool __fmt_file;
  ecl_file_enum file_type;

  file_type = ecl_util_get_file_type( filename , &__fmt_file , NULL);
  if (file_type == ECL_OTHER_FILE)
    __fmt_file = fmt_file;

  {
    fortio_type * target = fortio_open_writer( filename , __fmt_file , ECL_ENDIAN_FLIP);
    ecl_file_fwrite_fortio( ecl_file , target , 0);
    fortio_fclose( target );
  }
}









/*****************************************************************/
/**
   Here comes several functions for querying the ecl_file instance, and
   getting pointers to the ecl_kw content of the ecl_file. For getting
   ecl_kw instances there are two principally different access methods:

   * ecl_file_iget_named_kw(): This function will take a keyword
   (char *) and an integer as input. The integer corresponds to the
   ith occurence of the keyword in the file.

   * ecl_file_iget_kw(): This function just takes an integer index as
   input, and returns the corresponding ecl_kw instance - without
   considering which keyword it is.

   -------

   In addition the functions ecl_file_get_num_distinct_kw() and
   ecl_file_iget_distinct_kw() will return the number of distinct
   keywords, and distinct keyword keyword nr i (as a const char *).


   Possible usage pattern:

   ....
   for (ikw = 0; ikw < ecl_file_get_num_distinct_kw(ecl_file); ikw++) {
   const char * kw = ecl_file_iget_distinct_kw(ecl_file , ikw);

   printf("The file contains: %d occurences of \'%s\' \n",ecl_file_get_num_named_kw( ecl_file , kw) , kw);
   }
   ....

   For the summary file showed in the top this code will produce:

   The file contains 1 occurences of 'SEQHDR'
   The file contains 3 occurences of 'MINISTEP'
   The file contains 3 occurences of 'PARAMS'

*/









/**
   This function will iterate through the ecl_file instance and search
   for the ecl_kw instance @old_kw - the search is based on pointer
   equality, i.e. the actual ecl_kw instance, and not on content
   equality.

   When @old_kw is found that keyword instance will be discarded with
   ecl_kw_free() and the new keyword @new_kw will be inserted. If
   @old_kw can not be found the function will fail hard - to verify
   that @new_kw is indeed in the ecl_file instance you should use
   ecl_file_has_kw_ptr() first.

   The ecl_file function typically gives out references to the
   internal ecl_kw instances via the ecl_file_iget_kw() function. Use
   of ecl_file_replace_kw() might lead to invalidating ecl_kw
   instances held by the calling scope:


   ....
   ecl_file_type * restart_file   = ecl_file_fread_alloc( "ECLIPSE.UNRST" );
   ecl_kw_type * initial_pressure = ecl_file_iget_named_kw( ecl_file , "PRESSURE" , 0);
   ecl_kw_type * faked_pressure   = ecl_kw_alloc_copy( initial_pressure );

   ecl_kw_scale( faked_pressure , 1.25 );
   ecl_file_replace_kw( restart_file , initial_pressure , faked_pressure , false );  <--- This call will invalidate the inital_pressure reference
   ....
   ....
   // This will fail horribly:
   printf("The initial pressure in cell(0) was:%g \n",ecl_kw_iget_double( initial_pressure , 0 ));
   /|\
   |
   +---------> Using initial_pressure => Crash and burn!

   The ecl_file structure takes ownership of all the keywords, and
   will also take ownership of the newly instered @new_kw instance; if
   the boolean @insert_copy is set to true the function will insert a
   copy of @new_kw, leaving the original reference untouched.
*/



void ecl_file_replace_kw( ecl_file_type * ecl_file , ecl_kw_type * old_kw , ecl_kw_type * new_kw , bool insert_copy) {
  ecl_file_view_replace_kw( ecl_file->active_view , old_kw , new_kw , insert_copy );
}





ecl_kw_type * ecl_file_icopy_named_kw( const ecl_file_type * ecl_file , const char * kw, int ith) {
  return ecl_kw_alloc_copy( ecl_file_iget_named_kw( ecl_file , kw , ith ));
}


/*
  Will return the number of times a particular keyword occurs in a
  ecl_file instance. Will return 0 if the keyword can not be found.
*/

int ecl_file_get_num_named_kw(const ecl_file_type * ecl_file , const char * kw) {
  return ecl_file_view_get_num_named_kw( ecl_file->active_view , kw);
}






/**
   This function does the following:

   1. Takes an input index which goes in to the global kw_list vector.
   2. Looks up the corresponding keyword.
   3. Return the number of this particular keyword instance, among
   the other instance with the same header.

   With the example above we get:

   ecl_file_iget_occurence(ecl_file , 2) -> 0; Global index 2 will
   look up the first occurence of PARAMS.

   ecl_file_iget_occurence(ecl_file , 5) -> 2; Global index 5 will
   look up th third occurence of MINISTEP.

   The enkf layer uses this funny functionality.
*/


int ecl_file_iget_occurence( const ecl_file_type * ecl_file , int index) {
  return ecl_file_view_iget_occurence( ecl_file->active_view , index );
}


/**
    Returns the total number of ecl_kw instances in the ecl_file
    instance.
*/
int ecl_file_get_size( const ecl_file_type * ecl_file ){
  return ecl_file_view_get_size( ecl_file->active_view );
}


/**
   Returns true if the ecl_file instance has at-least one occurence of
   ecl_kw 'kw'.
*/
bool ecl_file_has_kw( const ecl_file_type * ecl_file , const char * kw) {
  return ecl_file_view_has_kw( ecl_file->active_view , kw );
}


int ecl_file_get_num_distinct_kw(const ecl_file_type * ecl_file) {
  return ecl_file_view_get_num_distinct_kw( ecl_file->active_view );
}


const char * ecl_file_iget_distinct_kw(const ecl_file_type * ecl_file, int index) {
  return ecl_file_view_iget_distinct_kw( ecl_file->active_view , index );
}


const char * ecl_file_get_src_file( const ecl_file_type * ecl_file ) {
  return fortio_filename_ref( ecl_file->fortio );
}


void ecl_file_fprintf_kw_list( const ecl_file_type * ecl_file , FILE * stream ) {
  ecl_file_view_fprintf_kw_list( ecl_file->active_view , stream );
}


/*****************************************************************/

ecl_file_kw_type * ecl_file_iget_file_kw( const ecl_file_type * file , int global_index) {
  return ecl_file_view_iget_file_kw( file->active_view , global_index);
}

ecl_file_kw_type * ecl_file_iget_named_file_kw( const ecl_file_type * file , const char * kw, int ith) {
  return ecl_file_view_iget_named_file_kw( file->active_view , kw, ith);
}

/* ---- */

ecl_kw_type * ecl_file_iget_kw( const ecl_file_type * file , int global_index) {
  return ecl_file_view_iget_kw( file->active_view , global_index);
}

ecl_type_enum ecl_file_iget_type( const ecl_file_type * file , int global_index) {
  return ecl_file_view_iget_type( file->active_view , global_index);
}

int ecl_file_iget_size( const ecl_file_type * file , int global_index) {
  return ecl_file_view_iget_size( file->active_view , global_index);
}

const char * ecl_file_iget_header( const ecl_file_type * file , int global_index) {
  return ecl_file_view_iget_header( file->active_view , global_index);
}

/* ---------- */

/*
   This function will return the ith occurence of 'kw' in
   ecl_file. Will abort hard if the request can not be satisifed - use
   query functions if you can not take that.
*/

ecl_kw_type * ecl_file_iget_named_kw( const ecl_file_type * file , const char * kw, int ith) {
  return ecl_file_view_iget_named_kw( file->active_view , kw , ith);
}

void ecl_file_indexed_read(const ecl_file_type * file , const char * kw, int index, const int_vector_type * index_map, char* buffer) {
    ecl_file_view_index_fload_kw(file->active_view, kw, index, index_map, buffer);
}

ecl_type_enum ecl_file_iget_named_type( const ecl_file_type * file , const char * kw , int ith) {
  return ecl_file_view_iget_named_type( file->active_view , kw , ith );
}

int ecl_file_iget_named_size( const ecl_file_type * file , const char * kw , int ith) {
  return ecl_file_view_iget_named_size( file->active_view , kw , ith );
}



/*****************************************************************/


static ecl_file_view_type * ecl_file_get_global_blockview( ecl_file_type * ecl_file , const char * kw , int occurence) {
  ecl_file_view_type * view = ecl_file_view_alloc_blockmap( ecl_file->global_view , kw , occurence );

  if (view)
    ecl_file_view_add_child( ecl_file->global_view , view );

  return view;
}

static ecl_file_view_type * ecl_file_get_relative_blockview( ecl_file_type * ecl_file , const char * kw , int occurence) {
  ecl_file_view_type * view = ecl_file_view_alloc_blockmap( ecl_file->active_view , kw , occurence );

  if (view)
    ecl_file_view_add_child( ecl_file->active_view , view );

  return view;
}




/*****************************************************************/
/*
  Different functions to open and close a file.
*/

/**
   The ecl_file_scan() function will scan through the whole file and
   build up an index of all the kewyords. The map created from this
   scan will be stored under the 'global_view' field; and all
   subsequent lookup operations will ultimately be based on the global
   map.
*/

static bool ecl_file_scan( ecl_file_type * ecl_file ) {
  bool scan_ok = false;
  fortio_fseek( ecl_file->fortio , 0 , SEEK_SET );
  {
    ecl_kw_type * work_kw = ecl_kw_alloc_new("WORK-KW" , 0 , ECL_INT_TYPE , NULL);

    while (true) {
      if (fortio_read_at_eof(ecl_file->fortio)) {
        scan_ok = true;
        break;
      }

      {
        offset_type current_offset = fortio_ftell( ecl_file->fortio );
        if (ecl_kw_fread_header( work_kw , ecl_file->fortio)) {
          ecl_file_kw_type * file_kw = ecl_file_kw_alloc( work_kw , current_offset);
          if (ecl_file_kw_fskip_data( file_kw , ecl_file->fortio ))
            ecl_file_view_add_kw( ecl_file->global_view , file_kw );
          else
            break;
        } else
          break;
      }
    }

    ecl_kw_free( work_kw );
  }
  if (scan_ok)
    ecl_file_view_make_index( ecl_file->global_view );

  return scan_ok;
}


void ecl_file_select_global( ecl_file_type * ecl_file ) {
  ecl_file->active_view = ecl_file->global_view;
}


/**
   The fundamental open file function; all alternative open()
   functions start by calling this one. This function will read
   through the complete file, extract all the keyword headers and
   create the map/index stored in the global_view field of the ecl_file
   structure. No keyword data will be loaded from the file.

   The ecl_file instance will retain an open fortio reference to the
   file until ecl_file_close() is called.
*/


ecl_file_type * ecl_file_open( const char * filename , int flags) {
  fortio_type * fortio;
  bool          fmt_file;

  ecl_util_fmt_file( filename , &fmt_file);

  if (ecl_file_view_flags_set(flags , ECL_FILE_WRITABLE))
    fortio = fortio_open_readwrite( filename , fmt_file , ECL_ENDIAN_FLIP);
  else
    fortio = fortio_open_reader( filename , fmt_file , ECL_ENDIAN_FLIP);

  if (fortio) {
    ecl_file_type * ecl_file = ecl_file_alloc_empty( flags );
    ecl_file->fortio = fortio;
    ecl_file->global_view = ecl_file_view_alloc( ecl_file->fortio , &ecl_file->flags , ecl_file->inv_view , true );

    if (ecl_file_scan( ecl_file )) {
      ecl_file_select_global( ecl_file );

      if (ecl_file_view_flags_set( ecl_file->flags , ECL_FILE_CLOSE_STREAM))
        fortio_fclose_stream( ecl_file->fortio );

      return ecl_file;
    } else {
      ecl_file_close( ecl_file );
      return NULL;
    }
  } else
    return NULL;
}





int ecl_file_get_flags( const ecl_file_type * ecl_file ) {
  return ecl_file->flags;
}

void ecl_file_set_flags( ecl_file_type * ecl_file, int flags ) {
  ecl_file->flags = flags;
}

bool ecl_file_flags_set( const ecl_file_type * ecl_file , int flags) {
  return ecl_file_view_flags_set( ecl_file->flags , flags );
}

bool ecl_file_writable( const ecl_file_type * ecl_file ) {
  return ecl_file_view_flags_set( ecl_file->flags , ECL_FILE_WRITABLE );
}


void ecl_file_push_block( ecl_file_type * ecl_file ) {
  vector_append_ref( ecl_file->map_stack , ecl_file->active_view );
}

void ecl_file_pop_block( ecl_file_type * ecl_file ) {
  ecl_file->active_view = vector_pop_back( ecl_file->map_stack );
}


bool ecl_file_subselect_block( ecl_file_type * ecl_file , const char * kw , int occurence) {
  ecl_file_view_type * blockmap = ecl_file_get_relative_blockview( ecl_file , kw , occurence);
  if (blockmap != NULL) {
    ecl_file->active_view = blockmap;
    return true;
  } else
    return false;
}


bool ecl_file_select_block( ecl_file_type * ecl_file , const char * kw , int occurence ) {
  ecl_file_view_type * blockmap = ecl_file_get_global_blockview( ecl_file , kw , occurence);
  if (blockmap != NULL) {
    ecl_file->active_view = blockmap;
    return true;
  } else
    return false;
}


/**
   The ecl_file_close() function will close the fortio instance and
   free all the data created by the ecl_file instance; this includes
   the ecl_kw instances which have been loaded on demand.
*/

void ecl_file_close(ecl_file_type * ecl_file) {
  if (ecl_file->fortio != NULL)
    fortio_fclose( ecl_file->fortio  );

  ecl_file_view_free( ecl_file->global_view );
  inv_map_free( ecl_file->inv_view );
  vector_free( ecl_file->map_stack );
  free( ecl_file );
}


void ecl_file_close_fortio_stream(ecl_file_type * ecl_file) {
    if (ecl_file->fortio != NULL) {
        fortio_fclose_stream(ecl_file->fortio);
    }
}


/**
   This function will detach the current ecl_file instance from the
   underlying fortio instance. The ecl_file instance can be used
   further to access the ecl_kw instances which have been loaded
   already, but if you try on-demand loading of a keyword you will get
   crash-and-burn. To ensure that all keywords are in memory you can
   call ecl_file_load_all() prior to the detach call.
*/


void ecl_file_fortio_detach( ecl_file_type * ecl_file ) {
  fortio_fclose( ecl_file->fortio );
  ecl_file->fortio = NULL;
}


bool ecl_file_load_all( ecl_file_type * ecl_file ) {
  return ecl_file_view_load_all( ecl_file->active_view );
}


void ecl_file_free__(void * arg) {
  ecl_file_close( ecl_file_safe_cast( arg ) );
}


/****************************************************************************/
/* Here we include two files with functions specialized to work with
   restart and summary files. Observe that the files ecl_rstfile.c and
   ecl_smryfile are compiled as part of the same compilation unit as
   ecl_file.c
*/

#include "ecl_rstfile.c"
#include "ecl_smryfile.c"

/*****************************************************************/
/* Two small lookup functions which consider the INTEHEAD keyword,
   work equally well for both restart and INIT files. */

/*
  The input @file must be either an INIT file or a restart file. Will
  fail hard if an INTEHEAD kw can not be found - or if the INTEHEAD
  keyword is not sufficiently large.

  The eclipse files can distinguish between ECLIPSE300 ( value == 300)
  and ECLIPSE300-Thermal option (value == 500). This function will
  return ECLIPSE300 in both those cases.
*/

ecl_version_enum ecl_file_get_ecl_version( const ecl_file_type * file ) {
  ecl_kw_type * intehead_kw = ecl_file_iget_named_kw( file , INTEHEAD_KW , 0 );
  int int_value = ecl_kw_iget_int( intehead_kw , INTEHEAD_IPROG_INDEX );

  if (int_value == INTEHEAD_ECLIPSE100_VALUE)
    return ECLIPSE100;
  else if ((int_value == INTEHEAD_ECLIPSE300_VALUE) || (int_value == INTEHEAD_ECLIPSE300THERMAL_VALUE))
    return ECLIPSE300;
  else {
    util_abort("%s: ECLIPSE version value:%d not recognized \n",__func__ , int_value );
    return -1;
  }
}

/*
  1: Oil
  2: Water
  3: Oil + water
  4: Gas
  5: Gas + Oil
  6: Gas + water
  7: Gas + Water + Oil
*/

int ecl_file_get_phases( const ecl_file_type * init_file ) {
  ecl_kw_type * intehead_kw = ecl_file_iget_named_kw( init_file , INTEHEAD_KW , 0 );
  int phases = ecl_kw_iget_int( intehead_kw , INTEHEAD_PHASE_INDEX );
  return phases;
}


/*
bool ecl_file_writable( const ecl_file_type * ecl_file ) {
  return fortio_writable( ecl_file->fortio );
}
*/

/**
   Checks if the ecl_file contains ecl_kw; this check is based on
   pointer equality - i.e. we check if the ecl_file contains exactly
   this keyword - not an arbitrary equivalent keyword.

   This function can be called as a safeguard before calling
   ecl_file_save_kw().
*/

bool ecl_file_has_kw_ptr( const ecl_file_type * ecl_file , const ecl_kw_type * ecl_kw) {
  ecl_file_kw_type * file_kw = inv_map_get_file_kw( ecl_file->inv_view , ecl_kw );
  if (file_kw == NULL)
    return false;
  else
    return true;
}


/*
  Will save the content of @ecl_kw to the on-disk file wrapped by the
  ecl_file instance. This function is quite strict:

  1. The actual keyword which should be updated is identified using
     pointer comparison; i.e. the ecl_kw argument must be the actual
     return value from an earlier ecl_file_get_kw() operation. To
     check this you can call ecl_file_has_kw_ptr().

  2. The header data of the ecl_kw must be unmodified; this is checked
     by the ecl_file_kw_inplace_fwrite() function and crash-and-burn
     will ensue if this is not satisfied.

  3. The ecl_file must have been opened with one of the _writable()
     open functions.
*/

bool ecl_file_save_kw( const ecl_file_type * ecl_file , const ecl_kw_type * ecl_kw) {
  ecl_file_kw_type * file_kw = inv_map_get_file_kw( ecl_file->inv_view , ecl_kw );  // We just verify that the input ecl_kw points to an ecl_kw
  if (file_kw != NULL) {                                                             // we manage; from then on we use the reference contained in
    if (fortio_assert_stream_open( ecl_file->fortio )) {                             // the corresponding ecl_file_kw instance.

      ecl_file_kw_inplace_fwrite( file_kw , ecl_file->fortio );

      if (ecl_file_view_flags_set( ecl_file->flags , ECL_FILE_CLOSE_STREAM))
        fortio_fclose_stream( ecl_file->fortio );

      return true;
    } else
      return false;
  } else {
    util_abort("%s: keyword pointer:%p not found in ecl_file instance. \n",__func__ , ecl_kw);
    return false;
  }
}


