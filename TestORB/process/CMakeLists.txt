target_sources (TestProcess PRIVATE
  TestDbConnect.cpp
  TestORB_Comp.cpp
  TestORB_I1.cpp
  TestORB_I2.cpp
  TestORB_I3.cpp
  TestORB.cpp
  TestPingPong.cpp
  TestUnion.cpp
)

# Causes 'section type conflict'
#target_precompile_headers (TestProcess PRIVATE 
#	${CMAKE_CURRENT_SOURCE_DIR}/pch.h)
