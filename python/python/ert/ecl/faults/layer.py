#  Copyright (C) 2014  Statoil ASA, Norway.
#
#  The file 'layer.py' is part of ERT - Ensemble based Reservoir Tool.
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

import ctypes
from cwrap import BaseCClass
from ert.ecl import EclPrototype
from ert.util import IntVector


class Layer(BaseCClass):
    TYPE_NAME = "layer"
    _alloc              = EclPrototype("void* layer_alloc(int,  int)", bind = False)
    _copy               = EclPrototype("void  layer_memcpy(layer , layer)")
    _free               = EclPrototype("void  layer_free(layer)")
    _get_nx             = EclPrototype("int   layer_get_nx(layer)")
    _get_ny             = EclPrototype("int   layer_get_ny(layer)")
    _set_cell           = EclPrototype("void  layer_iset_cell_value(layer , int , int , int)")
    _get_cell           = EclPrototype("int   layer_iget_cell_value(layer , int , int )")
    _get_bottom_barrier = EclPrototype("bool  layer_iget_bottom_barrier(layer , int , int )")
    _get_left_barrier   = EclPrototype("bool  layer_iget_left_barrier(layer , int , int )")
    _cell_contact       = EclPrototype("bool  layer_cell_contact(layer , int , int , int , int)")
    _add_barrier        = EclPrototype("void  layer_add_barrier(layer , int , int)")
    _add_ijbarrier      = EclPrototype("void  layer_add_ijbarrier(layer , int , int, int , int)")
    _add_interp_barrier = EclPrototype("void  layer_add_interp_barrier(layer , int , int)")
    _clear_cells        = EclPrototype("void  layer_clear_cells(layer)")
    _assign             = EclPrototype("void  layer_assign(layer , int)")
    _cell_sum           = EclPrototype("int   layer_get_cell_sum(layer)")
    _update_connected   = EclPrototype("void  layer_update_connected_cells(layer,int,int,int,int)")
    _cells_equal        = EclPrototype("void  layer_cells_equal( layer, int,int_vector,int_vector)")
    _count_equal        = EclPrototype("int   layer_count_equal( layer, int)")
    _active_cell        = EclPrototype("bool  layer_iget_active( layer, int,int)")
    _update_active      = EclPrototype("bool  layer_update_active( layer, ecl_grid , int)")

    def __init__(self , nx , ny):
        c_ptr = self._alloc( nx , ny )
        if c_ptr:
            super( Layer , self ).__init__(c_ptr)
        else:
            raise ValueError("Invalid input - no Layer object created")

    @classmethod
    def copy(cls , src):
        layer = Layer( src.getNX() , src.getNY())
        self._copy( layer , src )
        return layer


    def __assertIJ(self , i,j):
        if i < 0 or i >= self.getNX():
            raise ValueError("Invalid layer i:%d" % i)

        if j < 0 or j >= self.getNY():
            raise ValueError("Invalid layer j:%d" % j)


    def __unpackIndex(self , index):
        try:
            (i,j) = index
        except TypeError:
            raise ValueError("Index:%s is invalid - must have two integers" % str(index))

        self.__assertIJ(i,j)

        return (i,j)


    def __setitem__(self , index , value):
        (i,j) = self.__unpackIndex(index)
        self._set_cell( i , j , value )

    def activeCell(self , i,j):
        self.__assertIJ(i,j)
        return self._active_cell( i , j )


    def updateActive(self , grid , k):
        if grid.getNX() != self.getNX():
            raise ValueError("NX dimension mismatch. Grid:%d  layer:%d" % (grid.getNX() , self.getNX()))

        if grid.getNY() != self.getNY():
            raise ValueError("NY dimension mismatch. Grid:%d  layer:%d" % (grid.getNY() , self.getNY()))

        if k >= grid.getNZ():
            raise ValueError("K value invalid: Grid range [0,%d)" % grid.getNZ())

        self._update_active( grid , k )


    def __getitem__(self , index):
        (i,j) = self.__unpackIndex(index)
        return self._get_cell( i , j )

    def bottomBarrier(self , i,j):
        self.__assertIJ(i,j)
        return self._get_bottom_barrier( i , j )

    def leftBarrier(self , i,j):
        self.__assertIJ(i,j)
        return self._get_left_barrier( i , j )

    def getNX(self):
        return self._get_nx( )

    def getNY(self):
        return self._get_ny( )

    def free(self):
        self._free( )

    def cellContact(self , p1 , p2):
        i1,j1 = p1
        i2,j2 = p2

        if not 0 <= i1 < self.getNX():
            raise IndexError("Invalid i1:%d" % i1)

        if not 0 <= i2 < self.getNX():
            raise IndexError("Invalid i2:%d" % i2)

        if not 0 <= j1 < self.getNY():
            raise IndexError("Invalid i1:%d" % j1)

        if not 0 <= j2 < self.getNY():
            raise IndexError("Invalid i2:%d" % j2)

        return self._cell_contact( i1, j1, i2, j2 )


    def addInterpBarrier(self , c1 , c2):
        self._add_interp_barrier( c1 , c2 )


    def addPolylineBarrier(self , polyline , grid , k):
        if len(polyline) > 1:
            for i in range(len(polyline) - 1):
                x1,y1 = polyline[i]
                x2,y2 = polyline[i + 1]

                c1 = grid.findCellCornerXY( x1 , y1 , k )
                c2 = grid.findCellCornerXY( x2 , y2 , k )

                self.addInterpBarrier( c1 , c2 )


    def addFaultBarrier(self , fault , K , link_segments = True ):
        fault_layer = fault[K]
        num_lines = len(fault_layer)
        for index , fault_line in enumerate(fault_layer):
            for segment in fault_line:
                c1 , c2 = segment.getCorners()
                self._add_barrier( c1 , c2 )

            if index < num_lines - 1:
                next_line = fault_layer[index + 1]
                next_segment = next_line[0]
                next_c1 , next_c2 = next_segment.getCorners()

                if link_segments:
                    self.addInterpBarrier( c2 , next_c1 )


    def addIJBarrier(self , ij_list):
        if len(ij_list) < 2:
            raise ValueError("Must have at least two (i,j) points")

        nx = self.getNX()
        ny = self.getNY()
        p1 = ij_list[0]
        i1,j1 = p1
        for p2 in ij_list[1:]:
            i2,j2 = p2
            if i1 == i2 or j1 == j2:
                if not 0 <= i2 <= nx:
                    raise ValueError("i value:%d invalid. Valid range: [0,%d] " % (i , i2))

                if not 0 <= j2 <= ny:
                    raise ValueError("i value:%d invalid. Valid range: [0,%d] " % (j , j2))

                self._add_ijbarrier( i1 , j1 , i2 , j2 )
                p1 = p2
                i1,j1 = p1
            else:
                raise ValueError("Must have i1 == i2 or j1 == j2")


    def cellSum(self):
        return self._cell_sum( )

    def clearCells(self):
        """
        Will reset all cell and edge values to zero. Barriers will be left
        unchanged.
        """
        self._clear_cells( )


    def assign(self , value):
        """
        Will set the cell value to @value in all cells. Barriers will not be changed
        """
        self._assign( value )

    def updateConnected(self , ij , new_value , org_value = None):
        """
        Will update cell value of all cells in contact with cell ij to the
        value @new_value. If org_value is not supplied, the current
        value in cell ij is used.
        """
        if org_value is None:
            org_value = self[ij]

        if self[ij] == org_value:
            self._update_connected( ij[0] , ij[1] , org_value , new_value )
        else:
            raise ValueError("Cell %s is not equal to %d \n" % (ij , org_value))


    def cellsEqual(self , value):
        """
        Will return a list [(i1,j1),(i2,j2) , ...(in,jn)] of all cells with value @value.
        """
        i_list = IntVector()
        j_list = IntVector()
        self._cells_equal( value , i_list , j_list )
        ij_list= []
        for (i,j) in zip(i_list , j_list):
            ij_list.append( (i,j) )
        return ij_list


    def countEqual(self , value):
        return self._count_equal( value )
