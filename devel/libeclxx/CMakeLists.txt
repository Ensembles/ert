list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(CXX11Features REQUIRED)

add_subdirectory( src )

if (BUILD_TESTS)
  add_subdirectory( tests )
endif()
