cmake_minimum_required (VERSION 3.25)

set (CMAKE_CXX_SCAN_FOR_MODULES 0)
set (CMAKE_CONFIGURATION_TYPES Debug Release)

project (TestORB CXX)

include (NirvanaModule)

add_subdirectory (Console)
add_subdirectory (TestFixed)
add_subdirectory (TestORB)
add_subdirectory (TestSystem)
