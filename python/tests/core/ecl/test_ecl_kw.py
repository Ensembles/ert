#!/usr/bin/env python
#  Copyright (C) 2011  Statoil ASA, Norway. 
#   
#  The file 'test_kw.py' is part of ERT - Ensemble based Reservoir Tool.
#   
#  ERT is free software: you can redistribute it and/or modify 
#  it under the terms of the GNU General Public License as published by 
#  the Free Software Foundation, either version 3 of the License, or 
#  (at your option) any later version. 
#   
#  ERT is distributed in the hope that it will be useful, but WITHOUT ANY 
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or 
#  FITNESS FOR A PARTICULAR PURPOSE.   
#   
#  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
#  for more details.
import os
import random
import numpy
import warnings

from ert.ecl import EclKW, EclDataType, EclTypeEnum, EclFile, FortIO, EclFileFlagEnum , openFortIO

from ert.test import ExtendedTestCase , TestAreaContext
from cwrap import Stream

def copy_long():
    src = EclKW("NAME", 100, EclDataType.ECL_FLOAT)
    copy = src.sub_copy(0, 2000)


def copy_offset():
    src = EclKW("NAME", 100, EclDataType.ECL_FLOAT)
    copy = src.sub_copy(200, 100)


class KWTest(ExtendedTestCase):

    def test_min_max(self):
        kw = EclKW("TEST", 3, EclDataType.ECL_INT)
        kw[0] = 10
        kw[1] = 5
        kw[2] = 0

        self.assertEqual( 10 , kw.getMax())
        self.assertEqual( 0  , kw.getMin())
        self.assertEqual( (0,10)  , kw.getMinMax())

        
    def test_deprecated_datatypes(self):
        with warnings.catch_warnings(record=True) as w:
            warnings.simplefilter("always")
            kw = EclKW("Test", 10, EclTypeEnum.ECL_INT_TYPE)
            self.assertTrue(len(w) > 0)
            self.assertTrue(issubclass(w[-1].category, DeprecationWarning))

        with warnings.catch_warnings(record=True) as w:
            warnings.simplefilter("always")
            kw = EclKW("Test", 10, EclDataType.ECL_INT)
            self.assertTrue(len(w) == 0)

            self.assertEqual(EclTypeEnum.ECL_INT_TYPE, kw.type)

            self.assertTrue(len(w) > 0)
            self.assertTrue(issubclass(w[-1].category, DeprecationWarning))

    def kw_test( self, data_type, data, fmt ):
        name1 = "file1.txt"
        name2 = "file2.txt"
        kw = EclKW("TEST", len(data), data_type)
        i = 0
        for d in data:
            kw[i] = d
            i += 1

        file1 = Stream(name1, "w")
        kw.fprintf_data(file1, fmt)
        file1.close()

        file2 = Stream(name2, "w")
        for d in data:
            file2.write(fmt % d)
        file2.close()
        self.assertFilesAreEqual(name1, name2)
        self.assertEqual( kw.data_type , data_type )

    def test_create(self):
        with self.assertRaises(ValueError):
            EclKW( "ToGodDamnLong" , 100 , EclDataType.ECL_CHAR )



    def test_sum( self ):
        kw_string = EclKW( "STRING" , 100 , EclDataType.ECL_CHAR )
        with self.assertRaises(ValueError):
            kw_string.sum()


        kw_int = EclKW( "INT" , 4 , EclDataType.ECL_INT )
        kw_int[0] = 1
        kw_int[1] = 2
        kw_int[2] = 3
        kw_int[3] = 4
        self.assertEqual( kw_int.sum() , 10 )

        kw_d = EclKW( "D" , 4 , EclDataType.ECL_DOUBLE )
        kw_d[0] = 1
        kw_d[1] = 2
        kw_d[2] = 3
        kw_d[3] = 4
        self.assertEqual( kw_d.sum() , 10 )

        kw_f = EclKW( "F" , 4 , EclDataType.ECL_FLOAT )
        kw_f[0] = 1
        kw_f[1] = 2
        kw_f[2] = 3
        kw_f[3] = 4
        self.assertEqual( kw_f.sum() , 10 )

        kw_b = EclKW( "F" , 4 , EclDataType.ECL_BOOL )
        kw_b[0] = False
        kw_b[1] = True
        kw_b[2] = False
        kw_b[3] = True
        self.assertEqual( kw_b.sum() , 2 )



    def test_fprintf( self ):
        with TestAreaContext("python.ecl_kw"):
            self.kw_test(EclDataType.ECL_INT, [0, 1, 2, 3, 4, 5], "%4d\n")
            self.kw_test(EclDataType.ECL_FLOAT, [0.0, 1.1, 2.2, 3.3, 4.4, 5.5], "%12.6f\n")
            self.kw_test(EclDataType.ECL_DOUBLE, [0.0, 1.1, 2.2, 3.3, 4.4, 5.5], "%12.6f\n")
            self.kw_test(EclDataType.ECL_BOOL, [True, True, True, False, True], "%4d\n")
            self.kw_test(EclDataType.ECL_CHAR, ["1", "22", "4444", "666666", "88888888"], "%-8s\n")

    def test_kw_write(self):
        with TestAreaContext("python/ecl_kw/writing"):

            data = [random.random() for i in range(10000)]

            kw = EclKW("TEST", len(data), EclDataType.ECL_DOUBLE)
            i = 0
            for d in data:
                kw[i] = d
                i += 1

            pfx = 'EclKW('
            self.assertEqual(pfx, repr(kw)[:len(pfx)])

            fortio = FortIO("ECL_KW_TEST", FortIO.WRITE_MODE)
            kw.fwrite(fortio)
            fortio.close()

            fortio = FortIO("ECL_KW_TEST")

            kw2 = EclKW.fread(fortio)

            self.assertTrue(kw.equal(kw2))

            ecl_file = EclFile("ECL_KW_TEST", flags=EclFileFlagEnum.ECL_FILE_WRITABLE)
            kw3 = ecl_file["TEST"][0]
            self.assertTrue(kw.equal(kw3))
            ecl_file.save_kw(kw3)
            ecl_file.close()

            fortio = FortIO("ECL_KW_TEST", FortIO.READ_AND_WRITE_MODE)
            kw4 = EclKW.fread(fortio)
            self.assertTrue(kw.equal(kw4))
            fortio.seek(0)
            kw4.fwrite(fortio)
            fortio.close()

            ecl_file = EclFile("ECL_KW_TEST")
            kw5 = ecl_file["TEST"][0]
            self.assertTrue(kw.equal(kw5))



    def test_fprintf_data(self):
        with TestAreaContext("kw_no_header"):
            kw = EclKW("REGIONS" , 10 , EclDataType.ECL_INT)
            for i in range(len(kw)):
                kw[i] = i
                
            fileH = Stream("test" , "w")
            kw.fprintf_data( fileH )
            fileH.close()

            fileH = Stream("test" , "r")
            data = []
            for line in fileH.readlines():
                tmp = line.split()
                for elm in tmp:
                    data.append( int(elm) )

            for (v1,v2) in zip(data,kw):
                self.assertEqual(v1,v2)


    def test_sliced_set(self):
        kw = EclKW("REGIONS" , 10 , EclDataType.ECL_INT)
        kw.assign(99)
        kw[0:5] = 66
        self.assertEqual(kw[0] , 66)
        self.assertEqual(kw[4] , 66)
        self.assertEqual(kw[5] , 99)

        
    def test_long_name(self):
        with self.assertRaises(ValueError):
            EclKW("LONGLONGNAME" , 10 , EclDataType.ECL_INT)

        kw = EclKW("REGIONS" , 10 , EclDataType.ECL_INT)
        with self.assertRaises(ValueError):
            kw.setName("LONGLONGNAME")


    def test_abs(self):
        kw = EclKW("NAME" , 10 , EclDataType.ECL_CHAR)
        with self.assertRaises(TypeError):
            abs_kw = abs(kw)

        kw = EclKW("NAME" , 10 , EclDataType.ECL_BOOL)
        with self.assertRaises(TypeError):
            abs_kw = abs(kw)

        kw = EclKW("NAME" , 10 , EclDataType.ECL_INT)
        for i in range(len(kw)):
            kw[i] = -i

        abs_kw = abs(kw)
        for i in range(len(kw)):
            self.assertEqual(kw[i] , -i ) 
            self.assertEqual(abs_kw[i] , i ) 


    def test_fmt(self):
        kw1 = EclKW( "NAME1" , 100 , EclDataType.ECL_INT)
        kw2 = EclKW( "NAME2" , 100 , EclDataType.ECL_INT)

        for i in range(len(kw1)):
            kw1[i] = i + 1
            kw2[i] = len(kw1) - kw1[i]
            
        with TestAreaContext("ecl_kw/fmt") as ta:
            with openFortIO( "TEST.FINIT" , FortIO.WRITE_MODE , fmt_file = True ) as f:
                kw1.fwrite( f )
                kw2.fwrite( f )
                
            with openFortIO( "TEST.FINIT" , fmt_file = True ) as f:
                kw1b = EclKW.fread( f )
                kw2b = EclKW.fread( f )

            self.assertTrue( kw1 == kw1b )
            self.assertTrue( kw2 == kw2b )

            f = EclFile( "TEST.FINIT" )
            self.assertTrue( kw1 == f[0] )
            self.assertTrue( kw2 == f[1] )


    def test_first_different(self):
        kw1 = EclKW( "NAME1" , 100 , EclDataType.ECL_INT)
        kw2 = EclKW( "NAME2" , 100 , EclDataType.ECL_INT)
        kw3 = EclKW( "NAME2" , 200 , EclDataType.ECL_INT)
        kw4 = EclKW( "NAME2" , 100 , EclDataType.ECL_FLOAT)
        kw5 = EclKW( "NAME2" , 100 , EclDataType.ECL_FLOAT)

            
        with self.assertRaises( IndexError ):
            EclKW.firstDifferent( kw1 , kw2 , offset = 100 )

        with self.assertRaises( ValueError ):
            EclKW.firstDifferent( kw1 , kw3 )

        with self.assertRaises( TypeError ):
            EclKW.firstDifferent( kw1 , kw4 )

            
        with self.assertRaises( IndexError ):
            kw1.firstDifferent( kw2 , offset = 100 )

        with self.assertRaises( ValueError ):
            kw1.firstDifferent( kw3 )

        with self.assertRaises( TypeError ):
            kw1.firstDifferent( kw4 )


        kw1.assign( 1 )
        kw2.assign( 1 )

        self.assertEqual( kw1.firstDifferent( kw2 ) , len(kw1))

        kw1[0] = 100
        self.assertEqual( kw1.firstDifferent( kw2 ) , 0)
        self.assertEqual( kw1.firstDifferent( kw2 , offset = 1) , len(kw1))
        kw1[10] = 100
        self.assertEqual( kw1.firstDifferent( kw2 , offset = 1) , 10)


        kw4.assign( 1.0 )
        kw5.assign( 1.0 )
        self.assertEqual( kw4.firstDifferent( kw5 ) , len(kw4))

        kw4[10] *= 1.0001
        self.assertEqual( kw4.firstDifferent( kw5 ) , 10)

        self.assertEqual( kw4.firstDifferent( kw5 , epsilon = 1.0) , len(kw4))
        self.assertEqual( kw4.firstDifferent( kw5 , epsilon = 0.0000001) , 10)

        
    def test_numeric_equal(self):
        kw1 = EclKW("Name1" , 10 , EclDataType.ECL_DOUBLE )
        kw2 = EclKW("Name1" , 10 , EclDataType.ECL_DOUBLE )


        shift = 0.0001
        value = 1000

        abs_diff = shift
        rel_diff = shift / (shift + 2* value)
        kw1.assign( value )
        kw2.assign( value + shift )

        
        self.assertTrue( kw1.equal_numeric( kw2 , abs_epsilon = abs_diff * 1.1 , rel_epsilon = rel_diff * 1.1))
        self.assertFalse( kw1.equal_numeric( kw2 , abs_epsilon = abs_diff * 1.1 , rel_epsilon = rel_diff * 0.9))
        self.assertFalse( kw1.equal_numeric( kw2 , abs_epsilon = abs_diff * 0.9 , rel_epsilon = rel_diff * 1.1))
        self.assertTrue( kw1.equal_numeric( kw2 , abs_epsilon = 0 , rel_epsilon = rel_diff * 1.1))
        self.assertTrue( kw1.equal_numeric( kw2 , abs_epsilon = abs_diff * 1.1 , rel_epsilon = 0))

    def test_mul(self):
        kw1 = EclKW("Name1" , 10 , EclDataType.ECL_INT )
        kw1.assign( 10 )

        kw2 = EclKW("Name1" , 10 , EclDataType.ECL_INT )
        kw2.assign( 2 )

        kw3 = kw1 * kw2
        kw4 = kw1 + kw2
        self.assertEqual( len(kw3) , len(kw1))
        self.assertEqual( len(kw4) , len(kw1))
        for v in kw3:
            self.assertEqual( v , 20 )

        for v in kw4:
            self.assertEqual( v , 12 )


    def test_numpy(self):
        kw1 = EclKW("DOUBLE", 10, EclDataType.ECL_DOUBLE )
 
        view = kw1.numpyView( )
        copy = kw1.numpyCopy( )

        self.assertTrue( copy[ 0 ] == kw1[ 0 ] )
        self.assertTrue( view[ 0 ] == kw1[ 0 ] )

        kw1[ 0 ] += 1
        self.assertTrue( view[ 0 ] == kw1[ 0 ] )
        self.assertTrue( copy[ 0 ] == kw1[ 0 ] - 1)


        kw2 = EclKW("CHAR", 10, EclDataType.ECL_CHAR )
        with self.assertRaises(ValueError):
            kw2.numpyView( )

        kw3 = EclKW("BOOL", 10, EclDataType.ECL_BOOL )
        with self.assertRaises(ValueError):
            kw3.numpyView( )

    def test_slice(self):
        N = 100
        kw = EclKW("KW" , N , EclDataType.ECL_INT)
        for i in range(len(kw)):
            kw[i] = i

        even = kw[0:len(kw):2]
        odd  = kw[1:len(kw):2]

        self.assertEqual( len(even) , N/2 )
        self.assertEqual( len(odd)  , N/2 )

        for i in range(len(even)):
            self.assertEqual(even[i] , 2*i)
            self.assertEqual(odd[i] , 2*i + 1)


    def test_resize(self):
        N = 4
        kw = EclKW("KW" , N , EclDataType.ECL_INT)
        for i in range(N):
            kw[i] = i

        kw.resize( 2*N )
        self.assertEqual( len(kw) , 2*N )
        for i in range(N):
            self.assertEqual( kw[i] , i )

        kw.resize( N/2 )
        self.assertEqual( len(kw) , N/2 )
        for i in range(N/2):
            self.assertEqual( kw[i] , i )
            

    def test_typename(self):
        kw = EclKW("KW" , 100 , EclDataType.ECL_INT)

        self.assertEqual( kw.typeName( ) , "INTE")
