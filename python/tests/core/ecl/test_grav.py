import time
from ert.ecl import EclGrav, EclKW, EclGrid, EclFile, EclDataType, openFortIO, FortIO
from ert.test import ExtendedTestCase , TestAreaContext


class EclGravTest(ExtendedTestCase):


    def setUp(self):
        self.grid = EclGrid.createRectangular( (10,10,10) , (1,1,1))
        
        
    def test_create(self):
        # The init file created here only contains a PORO field. More
        # properties must be added to this before it can be used for
        # any usefull gravity calculations.
        poro = EclKW( "PORO" , self.grid.getGlobalSize() , EclDataType.ECL_FLOAT )
        with TestAreaContext("grav_init"):
            with openFortIO( "TEST.INIT" , mode = FortIO.WRITE_MODE ) as f:
                poro.fwrite( f )
            self.init = EclFile( "TEST.INIT")

            grav = EclGrav( self.grid , self.init )

    def test_aquifer(self):
        path = '/private/pgdr/opm/opm-data/norne/opm-simulation-reference/NORNE_ATW2013.'
        grid = EclGrid(path+'EGRID')
        print(repr(grid))
        init = EclFile(path+'FINIT')
        self.assertTrue(init.has_kw('AQUIFERN'))
        aquifer = EclGrav.aquifer(grid, init)
        print(len(aquifer))
        print(repr(aquifer))
        print(str(aquifer))
        self.assertEqual(13, sum(aquifer))
