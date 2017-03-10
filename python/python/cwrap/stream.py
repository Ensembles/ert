#  Copyright (C) 2017  Statoil ASA, Norway.
#
#  This file is part of ERT - Ensemble based Reservoir Tool.
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

from os import linesep
from six import string_types

import numpy
from .clib import load as cwrapload
from .prototype import Prototype
from .basecclass import BaseCClass


class LibcPrototype(Prototype):
    lib = cwrapload('libc', '.6')

    def __init__(self, prototype, bind=False):
        super(LibcPrototype, self).__init__(LibcPrototype.lib, prototype, bind=bind)

class Stream(BaseCClass):
    """
    Utility class to map a Python file handle <-> FILE* in C
    """
    TYPE_NAME = "stream"

    _fopen  = LibcPrototype("void* fopen(char*, char*)")
    _fread  = LibcPrototype("size_t   fread(void*, size_t, size_t, stream)")
    _fwrite = LibcPrototype("size_t   fwrite(void*, size_t, size_t, stream)")
    _fseek  = LibcPrototype("size_t   fseek(stream, size_t, size_t)", bind=True)

    _fclose = LibcPrototype("size_t   fclose (stream)", bind=True)
    _fflush = LibcPrototype("size_t   fflush (stream)", bind=True)

    def __init__(self, fname, mode='r'):
        c_ptr = self._fopen(fname, mode)
        self._mode = mode
        self._fname = fname

        self._closed = False # A closed file cannot be used for further I/O
                             # operations.  close() may be called more than once
                             # without error.

        try:
            super(Stream, self).__init__(c_ptr)
        except ValueError as e:
            self._closed = True
            raise IOError('Could not load file "%s" in mode %s.' % (fname, mode))

    @property
    def closed(self):
        return self._closed

    def _assert_open(self):
        if self.closed:
            raise IOError('Stream is closed: %s' % self)

    def _read_rest_of_file(self):
        out = numpy.zeros(0, dtype=numpy.byte)
        read = -1
        while read != 0: # num bytes read
            bptr = numpy.zeros(1, dtype=numpy.byte)
            read = self._fread(bptr.ctypes.data, 1, 1, self)
            if read:
                out = numpy.append(out, bptr)
        return out

    def read(self, size=-1):
        """Will read size bytes, or rest of file if size < 0.

        The return type will be iterable, but may be a string or a numpy bytes
        array.

        If the file is open in textmode (no 'b'), it will return a string.
        """
        self._assert_open()
        ret_arr = None
        if size < 0:
            ret_arr = self._read_rest_of_file()
        else:
            byte_array = numpy.zeros(size, dtype=numpy.byte)
            self._fread(byte_array.ctypes.data, 1, size, self)
            ret_arr = byte_array
        if self._textmode():
            return ret_arr.tostring()
        return ret_arr

    def _is_newline_chr(self, c, sep=None):
        if sep is None:
            return c == ord(linesep)
        return c == ord(sep)

    def readline(self, sep=None):
        """ returns string.  sep defaults to linesep=\n """
        self._assert_open()
        if not self._textmode():
            raise IOError('Warning: reading line from %s in byte mode is nonsensical.' % self)
        out = numpy.zeros(0, dtype=numpy.byte)
        while True:
            bptr = numpy.zeros(1, dtype=numpy.byte)
            read = self._fread(bptr.ctypes.data, 1, 1, self)
            if read == 0:
                break # EOF
            out = numpy.append(out, bptr)
            if self._is_newline_chr(bptr[0], sep):
                break
        return out.tostring()

    def readlines(self, sep=None):
        return [x for x in self.readline(sep=sep)]

    def __iter__(self):
        self._assert_open()
        prev = self.readline()
        while len(prev) > 0:
            yield prev
            prev = self.readline()

    def seek(self, offset, whence):
        """whence must be one of SEEK_SET, SEEK_CUR, or SEEK_END."""
        self._assert_open()
        if self._fseek(offset, whence):
            raise IOError("Unable to seek.")

    def _bytemode(self):
        return 'b' in self._mode

    def _textmode(self):
        return 'b' not in self._mode

    def _writable(self):
        return any(m in self._mode for m in 'aw+')

    def write(self, np_arr):
        """ @type np_arr: numpy.array """
        if not self._writable():
            raise IOError('Cannot write to file "%s" in mode %s.' % (self._fname, self._mode))
        if self.closed:
            raise IOError('File is closed.  Cannot write to file "%s".' % self._fname)

        if isinstance(np_arr, string_types):
            arr = numpy.zeros(len(np_arr), dtype=numpy.character)
            for i in range(len(np_arr)):
                arr[i] = np_arr[i]
            np_arr = arr
        return self._fwrite(np_arr.ctypes.data, 1, len(np_arr), self)

    def close(self):
        if not self.closed:
            self._fflush()
            cs = self._fclose()
            self._closed = True
            return cs

    def __repr__(self):
        cl = ', closed' if self.closed else ''
        fmt = 'fname=%s, mode=%s%s'
        return self._create_repr(fmt % (self._fname, self._mode, cl))

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()
        return exc_type is None

    def free(self):
        self.close()

    def __del__(self):
        self.close()
