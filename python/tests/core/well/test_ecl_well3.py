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
        sum = EclSum(sum_path)

        for wtl in well_info:
            for well_state in wtl:
                # print "%03d  %g   %g " % (R , well_state.oilRate(), sum.get_from_report( "WOPR:%s" % well , R))
                if wtl.getName() == "OP_4":
                    pass
                    # print well_state.oilRate(), well_state.waterRate(), well_state.gasRate()
                    # print sum.get_from_report("WOPR:%s" % wtl.getName(), 1)
                    # print sum.get_from_report( "WWPR:%s" % wtl.getName(), 30 )

                    # for conn in well_state.globalConnections():
                    #     print conn.gasRate(), conn.waterRate(), conn.oilRate()
                    #
                    # print sum.get_from_report("WGPR:%s" % wtl.getName(), 30)
                    #
                    # self.assertFloatEqual(well_state.oilRate(), sum.get_from_report("WOPR:%s" % wtl.getName(), 30))
                    # self.assertFloatEqual(well_state.waterRate(), sum.get_from_report("WWPR:%s" % wtl.getName(), 30))
                    # self.assertFloatEqual(well_state.gasRate(), sum.get_from_report("WGPR:%s" % wtl.getName(), 30))
