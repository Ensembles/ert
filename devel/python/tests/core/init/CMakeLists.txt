set(TEST_SOURCES
    __init__.py
    test_ert_init.py)

add_python_package("python.tests.init"  ${PYTHON_INSTALL_PREFIX}/tests/init "${TEST_SOURCES}" False)

addPythonTest(init.init_test init.test_ert_init.ErtInitTest )
