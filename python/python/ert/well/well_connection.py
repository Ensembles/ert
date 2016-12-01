from cwrap import BaseCClass
from ert.well import WellPrototype, WellConnectionDirectionEnum

class WellConnection(BaseCClass):
    TYPE_NAME = "well_connection"

    _i                   = WellPrototype("int    well_conn_get_i(well_connection)")
    _j                   = WellPrototype("int    well_conn_get_j(well_connection)")
    _k                   = WellPrototype("int    well_conn_get_k(well_connection)")
    _segment_id          = WellPrototype("int    well_conn_get_segment_id(well_connection)")
    _is_open             = WellPrototype("bool   well_conn_open(well_connection)")
    _is_msw              = WellPrototype("bool   well_conn_MSW(well_connection)")
    _fracture_connection = WellPrototype("bool   well_conn_fracture_connection(well_connection)")
    _matrix_connection   = WellPrototype("bool   well_conn_matrix_connection(well_connection)")
    _connection_factor   = WellPrototype("double well_conn_get_connection_factor(well_connection)")
    _equal               = WellPrototype("bool   well_conn_equal(well_connection, well_connection)")
    _get_dir             = WellPrototype("void*  well_conn_get_dir(well_connection)")

    def __init__(self):
        raise NotImplementedError("Class can not be instantiated directly")


    def isOpen(self):
        """ @rtype: bool """
        return self._is_open()


    def ijk(self):
        """ @rtype: tuple of (int, int, int) """
        i = self._i()
        j = self._j()
        k = self._k()
        return i, j, k

    def direction(self):
        """ @rtype: WellConnectionDirectionEnum """
        return self._get_dir()

    def segmentId(self):
        """ @rtype: int """
        return self._segment_id()

    def isFractureConnection(self):
        """ @rtype: bool """
        return self._fracture_connection()

    def isMatrixConnection(self):
        """ @rtype: bool """
        return self._matrix_connection()

    def connectionFactor(self):
        """ @rtype: float """
        return self._connection_factor()

    def __eq__(self, other):
        return self._equal(other)

    def __ne__(self, other):
        return not self == other

    def free(self):
        pass

    def isMultiSegmentWell(self):
        """ @rtype: bool """
        return self._is_msw()

    def __repr__(self):
        ijk = str(self.ijk())
        frac = 'fracture ' if self.isFractureConnection() else ''
        open_ = 'open ' if self.isOpen() else 'shut '
        msw = ' (multi segment)' if self.isMultiSegmentWell() else ''
        dir = WellConnectionDirectionEnum(self.direction())
        addr = self._address()
        return 'WellConnection(%s %s%s%s, direction = %s) at 0x%x' % (ijk, frac, open_, msw, dir, addr)
