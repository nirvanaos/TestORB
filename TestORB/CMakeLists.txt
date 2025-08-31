add_subdirectory (idl)

nirvana_module (MODULE_NAME TestModule IDL_FILES ${IDL_FILES})
add_subdirectory (module)
target_include_directories (TestModule PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

nirvana_test (MODULE_NAME TestProcess IDL_FILES ${IDL_FILES})
add_subdirectory (process)
target_include_directories (TestProcess PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
