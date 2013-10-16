set(PYTHON_SOURCES
    __init__.py
    ecl.py
    ecl_case.py
    ecl_default.py
    ecl_file.py
    ecl_grav.py
    ecl_grav_calc.py
    ecl_grid.py
    ecl_kw.py
    ecl_queue.py
    ecl_region.py
    ecl_rft.py
    ecl_rft_cell.py
    ecl_smspec_node.py
    ecl_subsidence.py
    ecl_sum.py
    ecl_sum_node.py
    ecl_sum_vector.py
    ecl_util.py
    fortio.py
)

add_python_package("python.ert.ecl"  ${PYTHON_INSTALL_PREFIX}/ert/ecl "${PYTHON_SOURCES}" True)

set(ECL_LOCAL_TARGET "" CACHE FILE "Name of optional external ecl_local module")

# if location isn't overridden in cache, then look in source tree
if (NOT EXISTS "${ECL_LOCAL_TARGET}"
    AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ecl_local.py")
    # note: only set temporarily, don't write to cache
    set(ECL_LOCAL_TARGET ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

# optionally add this file, if it was found
if (ECL_LOCAL_TARGET)
    add_python_package("Python ert.ecl.ecl_local"
                       ${PYTHON_INSTALL_PREFIX}/ert/ecl
                       "${ECL_LOCAL_TARGET}"
                       True)
else()
    message("** Hmmm - missing ecl_local.py module **")
endif()
