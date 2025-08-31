nirvana_test (MODULE_NAME TestSystem)
target_sources (TestSystem PRIVATE
  TestDomains.cpp
  TestESIOP.cpp
  TestFile.cpp
  TestNamingService.cpp
  TestSQLite.cpp
  TestSystem.cpp
)

target_precompile_headers (TestSystem PRIVATE pch.h)
