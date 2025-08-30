cmake_minimum_required (VERSION 3.25)

set (CMAKE_CXX_SCAN_FOR_MODULES 0)
set (CMAKE_CONFIGURATION_TYPES Debug Release)

project (TestORB CXX)

include (NirvanaModule)

add_subdirectory (idl)

nirvana_module (MODULE_NAME TestModule IDL_FILES ${IDL_FILES})
add_subdirectory (module)
target_include_directories (TestModule PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

nirvana_test (MODULE_NAME TestProcess IDL_FILES ${IDL_FILES})
add_subdirectory (process)
target_include_directories (TestProcess PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
