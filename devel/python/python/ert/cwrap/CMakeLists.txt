configure_file(clib.py.in clib.py)

set(PYTHON_SOURCES
    __init__.py
    basecclass.py
    basecenum.py
    basecvalue.py
    cclass.py
    cenum.py
    cfile.py
    ${CMAKE_CURRENT_BINARY_DIR}/clib.py
    cnamespace.py
    cwrap.py
)

add_python_package("python.ert.cwrap"  ${PYTHON_INSTALL_PREFIX}/ert/cwrap "${PYTHON_SOURCES}" True)

