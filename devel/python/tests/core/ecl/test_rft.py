#!/usr/bin/env python
#  Copyright (C) 2016  Statoil ASA, Norway. 
#   
#  The file 'test_rft.py' is part of ERT - Ensemble based Reservoir Tool.
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


import datetime
from ert.util import CTime
from ert.ecl import EclRFTFile, EclRFTCell, EclPLTCell
from ert.ecl.rft import WellTrajectory
from ert.test import ExtendedTestCase
from ert.ecl import EclRFT

class RFTTest(ExtendedTestCase):

    def test_create(self):
        rft = EclRFT( "WELL" , "RFT" , datetime.date(2015 , 10 , 1 ) , 100 )
        self.assertEqual( len(rft) , 0 )

        with self.assertRaises(IndexError):
            cell = rft[5]
            
        

