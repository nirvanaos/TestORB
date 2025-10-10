cmake_minimum_required (VERSION 3.25)

set (CMAKE_CXX_SCAN_FOR_MODULES 0)
set (CMAKE_CONFIGURATION_TYPES Debug Release)

project (Console CXX)

include (NirvanaSDK)

nirvana_module (Console MODULE_TYPE PROCESS)
target_sources (Console PRIVATE Console.cpp)
