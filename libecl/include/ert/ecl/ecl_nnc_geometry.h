/*
   Copyright (C) 2017  Statoil ASA, Norway.

   The file 'ecl_nnc_geometry.h' is part of ERT - Ensemble based Reservoir Tool.

   ERT is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   ERT is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.

   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
   for more details.
*/

#ifndef ERT_NNC_GEOMETRY_H
#define ERT_NNC_GEOMETRY_H
#ifdef __cplusplus
extern "C" {
#endif

#include <ert/util/type_macros.h>

#include <ert/ecl/ecl_grid.h>

typedef struct ecl_nnc_geometry_struct ecl_nnc_geometry_type;
typedef struct ecl_nnc_pair_struct ecl_nnc_pair_type;

struct ecl_nnc_pair_struct {
  int grid_nr1;
  int global_index1;
  int grid_nr2;
  int global_index2;

  int input_index; /* corresponds to the input ordering of this nnc */
};

UTIL_IS_INSTANCE_HEADER( ecl_nnc_geometry);
void                      ecl_nnc_geometry_free( ecl_nnc_geometry_type * nnc_geo);
ecl_nnc_geometry_type *   ecl_nnc_geometry_alloc( const ecl_grid_type * grid );
int                       ecl_nnc_geometry_size( const ecl_nnc_geometry_type * nnc_geo );
const ecl_nnc_pair_type * ecl_nnc_geometry_iget( const ecl_nnc_geometry_type * nnc_geo , int index);

#ifdef __cplusplus
}
#endif
#endif
