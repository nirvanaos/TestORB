target_sources (TestModule PRIVATE
	comp_factory.cpp
	DbConnectImpl.cpp
	I1_factory_dynamic.cpp
	I1_factory_portable.cpp
	I1_factory_stateless.cpp
	I1_factory_sysdomain.cpp
	I1_factory_tied.cpp
	I1_static.cpp
	I2_factory_dynamic.cpp
	I2_factory_sysdomain.cpp
	I3_factory_dynamic.cpp
	I3_factory_portable.cpp
	I3_factory_tied.cpp
	I3_static.cpp
	ImplAVT.cpp
	ImplI1.cpp
	ImplI2.cpp
	ImplV1.cpp
	ImplV2.cpp
	ImplV3.cpp
	PingPongImpl.cpp
)

# Causes 'section type conflict'
# target_precompile_headers (TestModule PRIVATE 
#	${CMAKE_CURRENT_SOURCE_DIR}/pch.h)
