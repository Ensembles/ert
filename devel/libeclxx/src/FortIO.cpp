/*
  Copyright 2015 Statoil ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdexcept>

#include <ert/ecl/fortio.h>
#include <ert/util/util.h>

#include <ert/ecl/FortIO.hpp>

namespace ERT {

    FortIO::FortIO(const std::string& filename , std::ios_base::openmode mode , bool fmt_file , bool endian_flip_header) {
        if (mode == std::ios_base::in) {
            if (util_file_exists( filename.c_str() )) {
                fortio_type * c_ptr = fortio_open_reader( filename.c_str() , fmt_file , endian_flip_header);
                m_fortio.reset( c_ptr );
            } else
                throw std::invalid_argument("File " + filename + " does not exist");
        } else {
            fortio_type * c_ptr = fortio_open_writer( filename.c_str() , fmt_file , endian_flip_header);
            m_fortio.reset( c_ptr );
        }
    }


    void FortIO::close() {
        if (m_fortio)
            m_fortio.reset( );
    }




    fortio_type * FortIO::getPointer() const {
        return m_fortio.get();
    }

}


