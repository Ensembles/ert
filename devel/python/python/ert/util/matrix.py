#  Copyright (C) 2011  Statoil ASA, Norway. 
#   
#  The file 'matrix.py' is part of ERT - Ensemble based Reservoir Tool. 
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


# The Matrix class implemented here wraps the C matrix implementation
# in matrix.c from the libutil library. The C matrix implementation
# has the very limited ambition of just barely satisfying the matrix
# needs of the EnKF algorithm, i.e. for general linear algebra
# applications you will probably be better served by a more complete
# matrix library. This applies even more so to this Python
# implementation; it is only here facilitate use of C libraries which
# expect a matrix instance as input (i.e. the LARS estimator). For
# general linear algebra in Python the numpy library is a natural
# choice.


from ert.cwrap import BaseCClass
from ert.util import UtilPrototype


class Matrix(BaseCClass):
    _matrix_alloc = UtilPrototype("void*  matrix_alloc(int, int )")
    _free         = UtilPrototype("void   matrix_free(matrix)")
    _iget         = UtilPrototype("double matrix_iget( matrix , int , int )")
    _iset         = UtilPrototype("void   matrix_iset( matrix , int , int , double)")
    _set_all      = UtilPrototype("void   matrix_scalar_set( matrix , double)")
    _scale_column = UtilPrototype("void matrix_scale_column(matrix , int , double)")
    _scale_row    = UtilPrototype("void matrix_scale_row(matrix , int , double)")
    _copy_column  = UtilPrototype("void matrix_copy_column(matrix , matrix , int , int)")
    _rows         = UtilPrototype("int matrix_get_rows(matrix)")
    _columns      = UtilPrototype("int matrix_get_columns(matrix)")
    _equal        = UtilPrototype("bool matrix_equal(matrix, matrix)")
    _pretty_print = UtilPrototype("void matrix_pretty_print(matrix, char*, char*)")
    _random_init  = UtilPrototype("void matrix_random_init(matrix, rng)")

    def __init__(self, rows, columns, value=0):
        c_ptr = self._matrix_alloc(rows, columns)
        super(Matrix, self).__init__(c_ptr)
        self.setAll(value)

    def __str__(self):
        s = ""
        for i in range(self.rows()):
            s += "["
            for j in range(self.columns()):
                d = self._iget(self, i, j)
                s += "%6.3g " % d
            s += "]\n"
        return s

    def __getitem__(self, index_tuple):
        if not 0 <= index_tuple[0] < self.rows():
            raise IndexError("Expected 0 <= %d < %d" % (index_tuple[0], self.rows()))

        if not 0 <= index_tuple[1] < self.columns():
            raise IndexError("Expected 0 <= %d < %d" % (index_tuple[1], self.columns()))

        return self._iget(self, index_tuple[0], index_tuple[1])

    def __setitem__(self, index_tuple, value):
        if not 0 <= index_tuple[0] < self.rows():
            raise IndexError("Expected 0 <= %d < %d" % (index_tuple[0], self.rows()))

        if not 0 <= index_tuple[1] < self.columns():
            raise IndexError("Expected 0 <= %d < %d" % (index_tuple[1], self.columns()))

        return self._iset(self, index_tuple[0], index_tuple[1], value)

    def dims(self):
        return self._rows(self), self._columns(self)

    def rows(self):
        """ @rtype: int """
        return self._rows(self)

    def columns(self):
        """ @rtype: int """
        return self._columns(self)

    def __eq__(self, other):
        assert isinstance(other, Matrix)
        return self._equal(self, other)

    def scaleColumn(self, column, factor):
        if not 0 <= column < self.columns():
            raise IndexError("Expected column: [0,%d) got:%d" % (self.columns(), column))
        self._scale_column(self, column, factor)

    def scaleRow(self, row, factor):
        if not 0 <= row < self.rows():
            raise IndexError("Expected row: [0,%d) got:%d" % (self.rows(), row))
        self._scale_row(self, row, factor)

    def setAll(self, value):
        self._set_all(self, value)

    def copyColumn(self, target_column, src_column):
        columns = self.columns()
        if not 0 <= src_column < columns:
            raise ValueError("src column:%d invalid" % src_column)

        if not 0 <= target_column < columns:
            raise ValueError("target column:%d invalid" % target_column)

        if src_column != target_column:
            # The underlying C function accepts column copy between matrices.
            self._copy_column(self, self, target_column, src_column)

    def prettyPrint(self, name, fmt="%6.3g"):
        self._pretty_print(self, name, fmt)

    def randomInit(self, rng):
        self._random_init(self, rng)

    def free(self):
        self._free(self)
