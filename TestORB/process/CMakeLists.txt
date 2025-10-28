target_sources (TestProcess PRIVATE
  TestDbConnect.cpp
  TestORB_Comp.cpp
  TestORB_I1.cpp
  TestORB_I2.cpp
  TestORB_I3.cpp
  TestORB.cpp
  TestPingPong.cpp
  TestUnion.cpp
  main.cpp
)

target_precompile_headers (TestProcess PRIVATE <CORBA/CORBA.h>)
