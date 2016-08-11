/*
   Copyright (C) 2012  Statoil ASA, Norway. 
    
   The file 'ert_util_stringlist_test.c' is part of ERT - Ensemble based Reservoir Tool. 
    
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

#include <ert/util/test_util.h>
#include <ert/util/stringlist.h>

void test_char() {
  const char * S1 = "S1";
  const char * S2 = "S2";
  const char * S3 = "S3";
  stringlist_type * s = stringlist_alloc_new();
  stringlist_append_ref( s , S1 );
  stringlist_append_ref( s , S2 );
  stringlist_append_ref( s , S3 );

  {
    char ** ref = stringlist_alloc_char_ref( s );
    char ** copy = stringlist_alloc_char_copy( s );
    int i;

    for (i=0; i < stringlist_get_size( s ); i++) {
      if (ref[i] != stringlist_iget(s , i))
        exit(1);

      if (strcmp( stringlist_iget( s , i ) , copy[i]) != 0)
        exit(1);

    }
  }
}



void test_reverse() {
  const char *s0 = "AAA";
  const char *s1 = "BBB";
  const char *s2 = "CCC";
  
  stringlist_type * s = stringlist_alloc_new();
  stringlist_append_ref( s , s0 );
  stringlist_append_ref( s , s1 );
  stringlist_append_ref( s , s2 );

  stringlist_reverse(s);

  test_assert_string_equal( s2 , stringlist_iget(s , 0 ));
  test_assert_string_equal( s1 , stringlist_iget(s , 1 ));
  test_assert_string_equal( s0 , stringlist_iget(s , 2 ));
}


void test_iget_as_int() {
  stringlist_type * s = stringlist_alloc_new();
  stringlist_append_ref(s , "1000" );
  stringlist_append_ref(s , "1000X" );
  stringlist_append_ref(s , "XXXX" );

  {
    int value;
    bool valid;

    value = stringlist_iget_as_int( s , 0 , &valid);
    test_assert_int_equal( value , 1000);
    test_assert_true( valid );
    
    value = stringlist_iget_as_int( s , 1 , &valid);
    test_assert_int_equal( value , -1);
    test_assert_false( valid );

    value = stringlist_iget_as_int( s , 2 , NULL);
    test_assert_int_equal( value , -1);
  }
}


void test_iget_as_double() {
  stringlist_type * s = stringlist_alloc_new();
  stringlist_append_ref(s , "1000.90" );
  stringlist_append_ref(s , "1000" );
  stringlist_append_ref(s , "XXXX" );

  {
    double value;
    bool valid;

    value = stringlist_iget_as_double( s , 0 , &valid);
    test_assert_double_equal( value , 1000.90);
    test_assert_true( valid );

    value = stringlist_iget_as_double( s , 1 , &valid);
    test_assert_double_equal( value , 1000.0);
    test_assert_true( valid );

    value = stringlist_iget_as_double( s , 2 , &valid);
    test_assert_double_equal( value , -1);
    test_assert_false( valid );
  }
}


void test_iget_as_bool() {
  stringlist_type * s = stringlist_alloc_new();
  stringlist_append_ref(s , "TRUE" );
  stringlist_append_ref(s , "True" );
  stringlist_append_ref(s , "true" );
  stringlist_append_ref(s , "T" );
  stringlist_append_ref(s , "1" );

  stringlist_append_ref(s , "FALSE" );
  stringlist_append_ref(s , "False" );
  stringlist_append_ref(s , "false" );
  stringlist_append_ref(s , "F" );
  stringlist_append_ref(s , "0" );

  stringlist_append_ref(s , "not_so_bool" );
  stringlist_append_ref(s , "8" );


  {
    bool value = false;
    bool valid = false;

    value = stringlist_iget_as_bool( s , 0 , &valid);
    test_assert_true( value );
    test_assert_true( valid );

    value = stringlist_iget_as_bool( s , 1 , &valid);
    test_assert_true( value );
    test_assert_true( valid );

    value = stringlist_iget_as_bool( s , 2 , &valid);
    test_assert_true( value );
    test_assert_true( valid );

    value = stringlist_iget_as_bool( s , 3 , &valid);
    test_assert_true( value );
    test_assert_true( valid );


    value = stringlist_iget_as_bool( s , 4 , &valid);
    test_assert_true( value );
    test_assert_true( valid );

    value = stringlist_iget_as_bool( s , 5 , &valid);
    test_assert_false( value );
    test_assert_true( valid );

    value = stringlist_iget_as_bool( s , 6 , &valid);
    test_assert_false( value );
    test_assert_true( valid );

    value = stringlist_iget_as_bool( s , 7 , &valid);
    test_assert_false( value );
    test_assert_true( valid );

    value = stringlist_iget_as_bool( s , 8 , &valid);
    test_assert_false( value );
    test_assert_true( valid );

    value = stringlist_iget_as_bool( s , 9 , &valid);
    test_assert_false( value );
    test_assert_true( valid );

    value = stringlist_iget_as_bool( s , 10 , &valid);
    test_assert_false( value );
    test_assert_false( valid );

    value = stringlist_iget_as_bool( s , 11 , &valid);
    test_assert_false( value );
    test_assert_false( valid );
  }
}


void test_empty() {
  stringlist_type * s = stringlist_alloc_new();
  stringlist_fprintf( s , "\n" , stdout );
}

void test_front_back() {
  stringlist_type * s = stringlist_alloc_new();

  stringlist_append_copy(s , "First");
  test_assert_string_equal("First" , stringlist_front(s));
  test_assert_string_equal("First" , stringlist_back(s));

  stringlist_append_copy(s , "Last");
  test_assert_string_equal("First" , stringlist_front(s));
  test_assert_string_equal("Last" , stringlist_back(s));
  stringlist_free( s );
}


void test_split() {
  stringlist_type *  s1 = stringlist_alloc_from_split("My Name    is Joakim Hove" , " ");
  test_assert_int_equal( 5 , stringlist_get_size( s1 ));
  test_assert_string_equal( "My" , stringlist_iget( s1 , 0 ));
  test_assert_string_equal( "Name" , stringlist_iget( s1  , 1 ));
  test_assert_string_equal( "is" , stringlist_iget( s1  , 2 ));
  test_assert_string_equal( "Joakim" , stringlist_iget( s1  , 3 ));
  test_assert_string_equal( "Hove" , stringlist_iget( s1  , 4 ));
  stringlist_free( s1 );

  
  s1 = stringlist_alloc_from_split("StringWithNoSPlit" , " ");
  test_assert_int_equal( 1 , stringlist_get_size( s1 ));
  test_assert_string_equal( "StringWithNoSPlit" , stringlist_iget( s1 , 0 ));
  stringlist_free( s1 );

  s1 = stringlist_alloc_from_split("A:B::C:D:" , ":");
  test_assert_int_equal( 4 , stringlist_get_size( s1 ));
  test_assert_string_equal( "A" , stringlist_iget( s1 , 0 ));
  test_assert_string_equal( "B" , stringlist_iget( s1 , 1 ));
  test_assert_string_equal( "C" , stringlist_iget( s1 , 2 ));
  test_assert_string_equal( "D" , stringlist_iget( s1 , 3 ));
  stringlist_free( s1 );

  s1 = stringlist_alloc_from_split("A:B::C:D:" , "::");
  test_assert_int_equal( 4 , stringlist_get_size( s1 ));
  test_assert_string_equal( "A" , stringlist_iget( s1 , 0 ));
  test_assert_string_equal( "B" , stringlist_iget( s1 , 1 ));
  test_assert_string_equal( "C" , stringlist_iget( s1 , 2 ));
  test_assert_string_equal( "D" , stringlist_iget( s1 , 3 ));
  stringlist_free( s1 );
}


void test_matching() {
  stringlist_type * s1 = stringlist_alloc_new();
  stringlist_type * s2 = stringlist_alloc_new();

  stringlist_append_copy(s1 , "AAA");
  stringlist_append_copy(s1 , "ABC" );
  stringlist_append_copy(s1 , "123");
  stringlist_append_copy(s1 , "ABC:123");

  stringlist_select_matching_elements( s2 , s1 , "*");
  test_assert_int_equal( 4 , stringlist_get_size( s2 ));
  test_assert_string_equal( "AAA" , stringlist_iget( s2 , 0 ));
  test_assert_string_equal( "ABC" , stringlist_iget( s2 , 1 ));
  test_assert_string_equal( "123" , stringlist_iget( s2 , 2 ));
  test_assert_string_equal( "ABC:123" , stringlist_iget( s2 , 3 ));

  stringlist_select_matching_elements( s2 , s1 , "*");
  test_assert_int_equal( 4 , stringlist_get_size( s2 ));
  test_assert_string_equal( "AAA" , stringlist_iget( s2 , 0 ));
  test_assert_string_equal( "ABC" , stringlist_iget( s2 , 1 ));
  test_assert_string_equal( "123" , stringlist_iget( s2 , 2 ));
  test_assert_string_equal( "ABC:123" , stringlist_iget( s2 , 3 ));
 

  stringlist_append_matching_elements( s2 , s1 , "*");
  test_assert_int_equal( 8 , stringlist_get_size( s2 ));
  test_assert_string_equal( "AAA" , stringlist_iget( s2 , 0 ));
  test_assert_string_equal( "ABC" , stringlist_iget( s2 , 1 ));
  test_assert_string_equal( "123" , stringlist_iget( s2 , 2 ));
  test_assert_string_equal( "ABC:123" , stringlist_iget( s2 , 3 ));

  test_assert_string_equal( "AAA" , stringlist_iget( s2 , 4 ));
  test_assert_string_equal( "ABC" , stringlist_iget( s2 , 5 ));
  test_assert_string_equal( "123" , stringlist_iget( s2 , 6 ));
  test_assert_string_equal( "ABC:123" , stringlist_iget( s2 , 7 ));
  
  stringlist_select_matching_elements( s2 , s1 , "*B*");
  test_assert_int_equal( 2 , stringlist_get_size( s2 ));
  test_assert_string_equal( "ABC" , stringlist_iget( s2 , 0 ));
  test_assert_string_equal( "ABC:123" , stringlist_iget( s2 , 1 ));

  stringlist_free( s2 );
  stringlist_free( s1 );
}


int main( int argc , char ** argv) {
  test_empty();
  test_char();
  test_reverse();
  test_iget_as_int();
  test_iget_as_bool();
  test_iget_as_double();
  test_split();
  test_matching();
  exit(0);
}
