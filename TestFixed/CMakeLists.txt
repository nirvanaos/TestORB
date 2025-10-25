cmake_minimum_required (VERSION 3.25)

set (CMAKE_CXX_SCAN_FOR_MODULES 0)
set (CMAKE_CONFIGURATION_TYPES Debug Release)

project (TestFixed CXX)

include (NirvanaSDK)

nirvana_test (TestFixed)
target_sources (TestFixed PRIVATE
  TestDecimal.cpp
  TestFixed.cpp
  main.cpp
)
