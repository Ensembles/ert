import datetime
import os.path

from ert.ecl import EclGrid, EclFile, EclSum
from ert.test import ExtendedTestCase
from ert.util.ctime import CTime
from ert.well import WellInfo, WellConnection, WellTypeEnum, WellConnectionDirectionEnum, WellSegment



class EclWellTest3(ExtendedTestCase):
    grid = None


    def test_rates(self):
        grid_path = self.createTestPath("Statoil/ECLIPSE/Gurbat/ECLIPSE.EGRID")
        rst_path = self.createTestPath("Statoil/ECLIPSE/Gurbat/ECLIPSE.UNRST")
        sum_path = self.createTestPath("Statoil/ECLIPSE/Gurbat/ECLIPSE.SMSPEC")

        grid = EclGrid(grid_path)
        well_info = WellInfo(grid, rst_path)
        sum = EclSum( sum_path )

        for well in ["OP_1"]:# , "OP_2", "OP_3", "OP_4" , "OP_5"]:
            for R in range(1,60):
                well_state = well_info[well][R]

                print "%03d  %g   %g " % (R , well_state.oilRate(), sum.get_from_report( "WOPR:%s" % well , R))
            
            #rint well_state.oilRate(), well_state.waterRate(), well_state.gasRate()
            #
            #rint sum.get_from_report( "WOPR:%s" % well , 30 )
            #rint sum.get_from_report( "WWPR:%s" % well , 30 )
            #rint sum.get_from_report( "WGPR:%s" % well , 30 )
            #
            #elf.assertFloatEqual( well_state.oilRate( ) , sum.get_from_report( "WOPR:%s" % well , 30 ))
            #elf.assertFloatEqual( well_state.waterRate( ) , sum.get_from_report( "WWPR:%s" % well , 30 ))
            #elf.assertFloatEqual( well_state.gasRate( ) , sum.get_from_report( "WGPR:%s" % well , 30 ))
            
