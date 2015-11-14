import os.path

import ert
from   ert.test import ExtendedTestCase

# This test should test functionality in the the root ert/__init__.py
# file.
class ErtInitTest(ExtendedTestCase):

    def test_PYTHONPATH(self):
        path = ert.PYTHONPATH 
        print path
        self.assertTrue( os.path.isdir( path ) )
        self.assertTrue( os.path.isdir( os.path.join( path , "ert")) )
        
        
        
