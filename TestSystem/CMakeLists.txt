cmake_minimum_required (VERSION 3.25)

set (CMAKE_CXX_SCAN_FOR_MODULES 0)
set (CMAKE_CONFIGURATION_TYPES Debug Release)

project (TestSystem CXX)

include (NirvanaSDK)

nirvana_test (TestSystem)

target_sources (TestSystem PRIVATE
  TestDomains.cpp
  TestESIOP.cpp
  TestFile.cpp
  TestNamingService.cpp
  TestSQLite.cpp
  TestSystem.cpp
  main.cpp
)

target_precompile_headers (TestSystem PRIVATE <Nirvana/Nirvana.h>)
