#  Copyright (C) 2011  Statoil ASA, Norway.
#
#  The file 'rng.py' is part of ERT - Ensemble based Reservoir Tool.
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


from ert.cwrap import BaseCClass
from ert.util import UtilPrototype
from ert.util.enums import RngInitModeEnum, RngAlgTypeEnum


class RandomNumberGenerator(BaseCClass):
    TYPE_NAME = "rng"

    _rng_alloc = UtilPrototype("void* rng_alloc(rng_alg_type_enum, rng_init_mode_enum)")
    _free = UtilPrototype("void rng_free(rng)")
    _get_double = UtilPrototype("double rng_get_double(rng)")
    _get_int = UtilPrototype("int rng_get_int(rng, int)")
    _get_max_int = UtilPrototype("uint rng_get_max_int(rng)")
    _state_size = UtilPrototype("int rng_state_size(rng)")
    _set_state = UtilPrototype("void rng_set_state(rng , char*)")

    def __init__(self, alg_type=RngAlgTypeEnum.MZRAN, init_mode=RngInitModeEnum.INIT_CLOCK):
        assert isinstance(alg_type, RngAlgTypeEnum)
        assert isinstance(init_mode, RngInitModeEnum)

        c_ptr = self._rng_alloc(alg_type, init_mode)
        super(RandomNumberGenerator, self).__init__(c_ptr)

    def stateSize(self):
        return self._state_size(self)

    def setState(self, seed_string):
        state_size = self.stateSize()
        if len(seed_string) < state_size:
            raise ValueError("The seed string must be at least %d characters long" % self.stateSize())
        self._set_state(self, seed_string)

    def getDouble(self):
        """ @rtype: float """
        return self._get_double(self)

    def getInt(self, max=None):
        """ @rtype: float """
        if max is None:
            max = self._get_max_int(self)

        return self._get_int(self, max)

    def free(self):
        self._free(self)
