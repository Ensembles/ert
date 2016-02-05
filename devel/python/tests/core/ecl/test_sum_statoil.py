#!/usr/bin/env python
#  Copyright (C) 2011  Statoil ASA, Norway. 
#   
#  The file 'sum_test.py' is part of ERT - Ensemble based Reservoir Tool. 
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
import datetime

from unittest import skipIf, skipUnless, skipIf

from ert.ecl import EclSum

from ert.util import StringList, TimeVector, DoubleVector

from ert.test import ExtendedTestCase , TestAreaContext
import csv

base = "ECLIPSE"
path = "Statoil/ECLIPSE/Gurbat"
case = "%s/%s" % (path, base)




def sum_get(*args):
    sum = args[0]
    key = args[1]
    vec = sum[key]


class SumTest(ExtendedTestCase):
    def setUp(self):
        self.case = self.createTestPath(case)
        self.ecl_sum = EclSum(self.case)

        self.assertIsInstance(self.ecl_sum, EclSum)


    def test_load(self):
        self.assertIsNotNone(self.ecl_sum, "Load failed")


    def test_invalid(self):
        with self.assertRaises(IOError):
            sum = EclSum("Does/not/exist")


    def test_KeyError(self):
        sum = self.ecl_sum
        with self.assertRaises(KeyError):
            v = sum["KeyMissing"]
        
        with self.assertRaises(KeyError):
            v = sum.get_interp("Missing" , days = 750)

        with self.assertRaises(KeyError):
            v = sum.get_interp_vector("Missing" , days_list = [750])



    def test_contains(self):
        self.assertTrue( "FOPT" in self.ecl_sum)
        self.assertFalse( "MISSING" in self.ecl_sum )


    def test_interp(self):
        sum = self.ecl_sum

        self.assertAlmostEqual(sum.get_interp("WWCT:OP_3", days=750), 0.11719122)
        self.assertAlmostEqual(sum.get_interp("WWCT:OP_3", date=datetime.date(2004, 1, 1)), 0.603358387947)

        v = sum.get_interp_vector("WOPT:OP_1", days_list=[100, 200, 400, 800])
        self.assertAlmostEqualList([805817.11875, 1614955.34677419, 3289267.67857143, 6493021.6218035], v)

        v = sum.get_interp_vector("WGPT:OP_2", date_list=[datetime.date(2002, 1, 1), datetime.date(2003, 1, 1), datetime.date(2004, 1, 1)])
        self.assertAlmostEqualList(v, [8.20773632e+08, 9.68444032e+08, 1.02515213e+09])

        self.assertEqual(sum.get_interp("FOPT" , days = 0) , 0)

        self.assertEqual(sum.get_interp("WOPR:OP_1" , days = 0) , 0)
        self.assertEqual(sum.get_interp("WOPR:OP_1" , date=datetime.date(2000,1,1)) , 0)

        self.assertEqual(sum.get_interp("WOPR:OP_1" , days = 31) , 7996)
        self.assertEqual(sum.get_interp("WOPR:OP_1" , date=datetime.date(2000,2,1)) , 7996)

        FPR = sum["FPR"]
        self.assertFloatEqual(sum.get_interp("FPR" , days = 0)  , FPR[0].value)
        self.assertFloatEqual(sum.get_interp("FPR" , days = 31) , FPR[1].value)

        with self.assertRaises(ValueError):
            sum.get_interp("WOPR:OP_1")

        with self.assertRaises(ValueError):
            sum.get_interp("WOPR:OP_1" , days=10 , date = datetime.date(2000,1,1))


    def test_LLINEAR(self):
        sum = EclSum( self.createTestPath("Statoil/ECLIPSE/Heidrun/LGRISSUE/EM-LTAA-ISEG_CARFIN_NWPROPS"))
        self.assertTrue( sum.has_key("LLINEARS") )

        

    def test_wells(self):
        wells = self.ecl_sum.wells()
        wells.sort()
        self.assertListEqual([well for well in wells], ["OP_1", "OP_2", "OP_3", "OP_4", "OP_5", "WI_1", "WI_2", "WI_3"])

        wells = self.ecl_sum.wells(pattern="*_3")
        wells.sort()
        self.assertListEqual([well for well in wells], ["OP_3", "WI_3"])

        groups = self.ecl_sum.groups()
        groups.sort()
        self.assertListEqual([group for group in groups], ['GMWIN', 'OP', 'WI'])


    def test_last( self ):
        last = self.ecl_sum.get_last("FOPT")
        self.assertFloatEqual(last.value, 38006336.0)
        self.assertFloatEqual(last.days, 1826.0)
        self.assertEqual(last.date, datetime.datetime(2004, 12, 31, 0, 0, 0))

        self.assertFloatEqual(self.ecl_sum.get_last_value("FGPT"), 6605249024.0)
        self.assertEqual( len(self.ecl_sum) , 63 )


    def test_dates( self ):
        sum = self.ecl_sum
        d = sum.dates

        self.assertEqual(d[0], datetime.datetime(2000, 1, 1, 0, 0, 0))
        self.assertEqual(d[62], datetime.datetime(2004, 12, 31, 0, 0, 0))
        self.assertEqual(len(d), 63)
        self.assertEqual(d[25], datetime.datetime(2001, 12, 1, 0, 0, 0))
        self.assertEqual(sum.iget_date(25), datetime.datetime(2001, 12, 1, 0, 0, 0))

        mpl_dates = sum.mpl_dates
        self.assertAlmostEqual(mpl_dates[25], 730820)

        days = sum.days
        self.assertAlmostEqual(days[50], 1461)

        self.assertEqual(sum.start_time, datetime.datetime(2000, 1, 1, 0, 0, 0))
        self.assertEqual(sum.end_time, datetime.datetime(2004, 12, 31, 0, 0, 0))
        self.assertTrue(sum.check_sim_time(datetime.datetime(2004, 12, 31, 0, 0, 0)))
        self.assertEqual(sum.end_date , datetime.date(2004, 12, 31))
        


    def test_dates2( self ):
        sum = EclSum(self.createTestPath("Statoil/ECLIPSE/FF12/FF12_2013B3_AMAP2"))
        self.assertEqual(sum.end_date , datetime.date(2045, 1, 1))





    def test_keys(self):
        sum = self.ecl_sum
        self.assertRaises(KeyError, sum.__getitem__, "BJARNE")

        v = sum["FOPT"]
        self.assertEqual(len(v), 63)


    def test_index(self):
        sum = self.ecl_sum
        index = sum.get_key_index("TCPUDAY")
        self.assertEqual(index, 10239)


    def test_report(self):
        sum = self.ecl_sum
        self.assertEqual(sum.get_report(date=datetime.date(2000, 10, 1)), 10)
        self.assertEqual(sum.get_report(date=datetime.date(2000, 10, 3)), -1)
        self.assertEqual(sum.get_report(date=datetime.date(1980, 10, 3)), -1)
        self.assertEqual(sum.get_report(date=datetime.date(2012, 10, 3)), -1)

        self.assertEqual(sum.get_report(days=91), 3)
        self.assertEqual(sum.get_report(days=92), -1)
        self.assertAlmostEqual(sum.get_interp("FOPT", days=91), sum.get_from_report("FOPT", 3))

        self.assertEqual(sum.first_report, 1)
        self.assertEqual(sum.last_report, 62)

        self.assertEqual(sum.get_report_time(10), datetime.date(2000, 10, 1))
        self.assertFloatEqual(sum.get_from_report("FOPT", 10), 6.67447e+06)


    @skipIf(ExtendedTestCase.slowTestShouldNotRun(), "Slow test skipped")
    def test_fwrite(self):
        # todo: What is tested here?
        # work_area = TestArea("python/sum-test/fwrite", True)
        with TestAreaContext("python/sum-test/fwrite") as work_area:
            self.ecl_sum.fwrite(ecl_case="CASE")
            self.assertTrue(True)


    def test_block(self):
        sum = self.ecl_sum
        index_ijk = sum.get_key_index("BPR:15,28,1")
        index_num = sum.get_key_index("BPR:1095")
        self.assertEqual(index_ijk, index_num)


    def test_restart(self):
        hist = EclSum(self.createTestPath("Statoil/ECLIPSE/sum-restart/history/T07-4A-W2011-18-P1"))
        base = EclSum(self.createTestPath("Statoil/ECLIPSE/sum-restart/prediction/BASECASE"))
        pred = EclSum(self.createTestPath("Statoil/ECLIPSE/sum-restart/prediction/BASECASE"), include_restart=False)

        self.assertIsNotNone(hist)
        self.assertIsNotNone(base)
        self.assertIsNotNone(pred)


    def test_case1(self ):
        self.assertTrue(self.ecl_sum.path == self.createTestPath(path))
        self.assertTrue(self.ecl_sum.base == base)
        self.assertTrue(self.ecl_sum.case == self.createTestPath(case))
        self.assertTrue(self.ecl_sum.abs_path == self.createTestPath(path))


    def test_case2( self ):
        cwd = os.getcwd()
        os.chdir(self.createTestPath(path))
        sum = EclSum(base)
        self.assertIsNone(sum.path)
        self.assertTrue(sum.base == base)
        self.assertTrue(sum.case == base)
        self.assertTrue(sum.abs_path == self.createTestPath(path))
        os.chdir(cwd)


    def test_var_properties( self ):
        sum = self.ecl_sum
        self.assertRaises(KeyError, sum.smspec_node, "BJARNE")

        node = sum.smspec_node("FOPT")
        self.assertTrue(node.is_total)
        self.assertFalse(node.is_historical)

        node = sum.smspec_node("FOPR")
        self.assertFalse(node.is_total)
        self.assertFalse(node.is_historical)
        self.assertTrue(node.keyword == "FOPR")

        node = sum.smspec_node("FOPRH")
        self.assertFalse(node.is_total)
        self.assertTrue(node.is_historical)
        self.assertTrue(node.is_rate)
        self.assertTrue(node.keyword == "FOPRH")

        node = sum.smspec_node("WOPR:OP_1")
        self.assertFalse(node.is_total)
        self.assertTrue(node.is_rate)
        self.assertTrue(node.keyword == "WOPR")

        node = sum.smspec_node("WOPT:OP_1")
        self.assertTrue(node.is_total)
        self.assertFalse(node.is_rate)
        self.assertTrue(node.unit == "SM3")
        self.assertTrue(node.wgname == "OP_1")
        self.assertTrue(node.keyword == "WOPT")

        self.assertTrue(sum.unit("FOPR") == "SM3/DAY")

        node = sum.smspec_node("FOPTH")
        self.assertTrue(node.is_total)
        self.assertFalse(node.is_rate)
        self.assertIsNone(node.wgname)

        node = sum.smspec_node("FOPTH")
        self.assertIsNone(node.num)

        node = sum.smspec_node("BPR:1095")
        self.assertEquals(node.num, 1095)

    def test_stringlist_gc(self):
        sum = EclSum(self.case)
        wells = sum.wells()
        well1 = wells[0]
        del wells
        self.assertTrue(well1 == "OP_1")


    def test_stringlist_reference(self):
        sum = EclSum(self.case)
        wells = sum.wells()
        self.assertListEqual([well for well in wells], ['OP_1', 'OP_2', 'OP_3', 'OP_4', 'OP_5', 'WI_1', 'WI_2', 'WI_3'])
        self.assertIsInstance(wells, StringList)


    def test_stringlist_setitem(self):
        sum = EclSum(self.case)
        wells = sum.wells()
        wells[0] = "Bjarne"
        well0 = wells[0]
        self.assertTrue(well0 == "Bjarne")
        self.assertTrue(wells[0] == "Bjarne")
        wells[0] = "XXX"
        self.assertTrue(well0 == "Bjarne")
        self.assertTrue(wells[0] == "XXX")


    def test_segment(self):
        sum = EclSum(self.createTestPath("Statoil/ECLIPSE/Oseberg/F8MLT/F8MLT-F4"))
        segment_vars = sum.keys("SOFR:F-8:*")
        self.assertIn("SOFR:F-8:1", segment_vars)
        for var in segment_vars:
            tmp = var.split(":")
            nr = int(tmp[2])
            self.assertTrue(nr >= 0)

    def test_return_types(self):
        self.assertIsInstance(self.ecl_sum.alloc_time_vector(True), TimeVector)
        key_index = self.ecl_sum.get_general_var_index("FOPT")
        self.assertIsInstance(self.ecl_sum.alloc_data_vector(key_index, True), DoubleVector)

    def test_timeRange(self):
        sum = EclSum(self.case)
        with self.assertRaises(TypeError):
            trange = sum.timeRange(interval = "1")
            trange = sum.timeRange(interval = "1X")
            trange = sum.timeRange(interval = "YY")
            trange = sum.timeRange(interval = "MY")

        with self.assertRaises(ValueError):
            trange = sum.timeRange( start = datetime.datetime(2000,1,1) , end = datetime.datetime(1999,1,1) )

        sim_start = datetime.datetime(2000, 1, 1, 0, 0, 0)
        sim_end = datetime.datetime(2004, 12, 31, 0, 0, 0)
        trange = sum.timeRange( interval = "1Y")
        self.assertTrue( trange[0] == datetime.date( 2000 , 1 , 1 ))
        self.assertTrue( trange[1] == datetime.date( 2001 , 1 , 1 ))
        self.assertTrue( trange[2] == datetime.date( 2002 , 1 , 1 ))
        self.assertTrue( trange[3] == datetime.date( 2003 , 1 , 1 ))
        self.assertTrue( trange[4] == datetime.date( 2004 , 1 , 1 ))
        self.assertTrue( trange[5] == datetime.date( 2005 , 1 , 1 ))

        trange = sum.timeRange( interval = "1M")
        self.assertTrue( trange[0] == datetime.date( 2000 , 1 , 1 ))
        self.assertTrue( trange[-1] == datetime.date( 2005 , 1 , 1 ))

        trange = sum.timeRange( start = datetime.date( 2002 , 1 , 15), interval = "1M")
        self.assertTrue( trange[0] == datetime.date( 2002 , 1 , 1 ))
        self.assertTrue( trange[-1] == datetime.date( 2005 , 1 , 1 ))

        trange = sum.timeRange( start = datetime.date( 2002 , 1 , 15) , end = datetime.date( 2003 , 1 , 15), interval = "1M")
        self.assertTrue( trange[0] == datetime.date( 2002 , 1 , 1 ))
        self.assertTrue( trange[-1] == datetime.date( 2003 , 2 , 1 ))

        trange = sum.timeRange( start = datetime.date( 2002 , 1 , 15) , end = datetime.datetime( 2003 , 1 , 15,0,0,0), interval = "1M")
        self.assertTrue( trange[0] == datetime.date( 2002 , 1 , 1 ))
        self.assertTrue( trange[-1] == datetime.date( 2003 , 2 , 1 ))



    # Loading this dataset is a test of loading a case where one report step is missing.
    def test_Heidrun(self):
        sum = EclSum( self.createTestPath("Statoil/ECLIPSE/Heidrun/Summary/FF12_2013B3_CLEAN_RS"))
        self.assertEqual( 452 , len(sum))
        self.assertFloatEqual( 1.8533144e+8 , sum.get_last_value("FOPT"))

        trange = sum.timeRange( start = datetime.date( 2015 , 1 , 1), interval = "1M")
        self.assertTrue( trange[0] == datetime.date( 2016 , 2 , 1 ))
        for t in trange:
            sum.get_interp( "FOPT" , date = t )
        

        with TestAreaContext("csv/export"):
            sum.exportCSV("file.csv")
            input_file = csv.DictReader( open("file.csv"))
            for row in input_file:
                keys = sum.keys( pattern = "W*")
                keys |= sum.keys( pattern = "G")
                
                for key in keys:
                    self.assertTrue( key in row )
                break

            

    def test_regularProduction(self):
        sum = EclSum(self.case)
        with self.assertRaises(TypeError):
            trange = TimeVector.createRegular( sum.start_time , sum.end_time , "1M" )
            prod = sum.blockedProduction("FOPR" , trange)
            
        with self.assertRaises(KeyError):
            trange = TimeVector.createRegular( sum.start_time , sum.end_time , "1M" )
            prod = sum.blockedProduction("NoNotThis" , trange)

        trange = sum.timeRange(interval = "2Y")
        self.assertTrue( trange[0]  == datetime.date( 2000 , 1 , 1 ))
        self.assertTrue( trange[-1] == datetime.date( 2006 , 1 , 1 ))

        trange = sum.timeRange(interval = "5Y")
        self.assertTrue( trange[0]  == datetime.date( 2000 , 1 , 1 ))
        self.assertTrue( trange[-1] == datetime.date( 2005 , 1 , 1 ))
        
        trange = sum.timeRange(interval = "6M")
        wprod1 = sum.blockedProduction("WOPT:OP_1" , trange)
        wprod2 = sum.blockedProduction("WOPT:OP_2" , trange)
        wprod3 = sum.blockedProduction("WOPT:OP_3" , trange)
        wprod4 = sum.blockedProduction("WOPT:OP_4" , trange)
        wprod5 = sum.blockedProduction("WOPT:OP_5" , trange)
    
        fprod = sum.blockedProduction("FOPT" , trange)
        gprod = sum.blockedProduction("GOPT:OP" , trange)
        wprod = wprod1 + wprod2 + wprod3 + wprod4 + wprod5
        for (w,f,g) in zip(wprod, fprod,gprod):
            self.assertFloatEqual( w , f )
            self.assertFloatEqual( w , g )



    def test_writer(self):
        writer = EclSum.writer("CASE" , datetime.date( 2000 , 1 , 1) , 10 , 10 , 5)
        self.assertIsInstance(self.ecl_sum, EclSum)

        
        writer.addVariable( "FOPT" )
        self.assertTrue( writer.has_key( "FOPT" ))
        
        writer.addTStep( 1 , 100 )
        
