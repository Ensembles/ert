/*
   Copyright (C) 2014  Statoil ASA, Norway. 
    
   The file 'ert_util_struct_vector.c' is part of ERT - Ensemble based Reservoir Tool. 
    
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

#include "ert/util/build_config.h"
#include <ert/util/test_util.h>
#include <ert/util/util.h>
#include <ert/util/struct_vector.h>



struct test_struct {
  int x;
  double y;
  int z;
};



void test_create() {
  struct test_struct d;
  struct_vector_type * struct_vector = struct_vector_alloc( sizeof d );
  test_assert_true( struct_vector_is_instance( struct_vector ));
  test_assert_int_equal( struct_vector_get_size( struct_vector ) , 0 );
  struct_vector_reserve( struct_vector , 1000 );
  test_assert_int_equal( struct_vector_get_size( struct_vector ) , 0 );
  struct_vector_free( struct_vector );
}

void alloc_invalid() {
  struct_vector_alloc( 0 );
}


void test_create_invalid() {
  test_assert_util_abort( "struct_vector_alloc" , alloc_invalid , NULL );
}

void test_append_iget() {
  struct test_struct d1,d2;
  struct_vector_type * struct_vector = struct_vector_alloc( sizeof d1 );
  d1.x = 100;
  d1.y = 99;
  d1.z = 234;

  struct_vector_append( struct_vector , &d1 );
  test_assert_int_equal( struct_vector_get_size( struct_vector ) , 1 );
  struct_vector_reserve( struct_vector , 0 );
  test_assert_int_equal( struct_vector_get_size( struct_vector ) , 1 );

  test_assert_false( d1.x == d2.x );
  test_assert_false( d1.y == d2.y );
  test_assert_false( d1.z == d2.z );

  struct_vector_iget( struct_vector , 0 , &d2);
  test_assert_true( d1.x == d2.x );
  test_assert_true( d1.y == d2.y );
  test_assert_true( d1.z == d2.z );

  {
    struct test_struct * d = struct_vector_get_data( struct_vector );
    struct test_struct d3 = d[0];
    test_assert_true( d1.x == d3.x );
    test_assert_true( d1.y == d3.y );
    test_assert_true( d1.z == d3.z );
  }
  struct_vector_reset( struct_vector );
  test_assert_int_equal( struct_vector_get_size( struct_vector ) , 0 );

  struct_vector_free( struct_vector );
}



int cmp( const void * _d1, const void * _d2) {
  const struct test_struct * d1 = (const struct test_struct *) _d1;
  const struct test_struct * d2 = (const struct test_struct *) _d2;

  if (d1->x > d2->x)
    return 1;

  if (d1->x < d2->x)
    return -1;

  return 0;
}


int rcmp( const void * _d1, const void * _d2) {
  return cmp(_d2 , _d1);
}



void test_sort() {
  struct test_struct d;
  struct_vector_type * struct_vector = struct_vector_alloc( sizeof d );
  for (int i = 0; i < 10; i++) {
    struct test_struct d = {.x = 9 - i,
                            .y = 9 - i,
                            .z = 9 - i };
    struct_vector_append( struct_vector , &d );
  }
  struct_vector_sort( struct_vector , cmp );
  for (int i = 0; i < 10; i++) {
    struct test_struct d;
    struct_vector_iget( struct_vector , i  ,&d );
    test_assert_int_equal( d.x , i );
    test_assert_int_equal( d.z , i );
  }
  struct_vector_sort( struct_vector , rcmp );
  for (int i = 0; i < 10; i++) {
    struct test_struct d;
    struct_vector_iget( struct_vector , i  ,&d );
    test_assert_int_equal( d.x , 9 - i );
    test_assert_int_equal( d.z , 9 - i );
  }
}



int main(int argc , char ** argv) {
  test_create();
  test_create_invalid();
  test_append_iget();
  test_sort();
}
