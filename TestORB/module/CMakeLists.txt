idl_generate(OUT_VAR GENERATED_FILES IDL_FILES ${IDL_FILES})

add_library(TestModule SHARED ${GENERATED_FILES})

target_sources(TestModule PRIVATE
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

target_include_directories (TestModule PRIVATE ${NIRVANA_INCLUDES}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries (TestModule PRIVATE ${NIRVANA_LIBS})

target_compile_options (TestModule PRIVATE
	"-nostdinc"
	"-U_WIN32"
	"-D_LIBCPP_MSVCRT_LIKE"
	"-D_LIBCPP_OBJECT_FORMAT_COFF"
	"--target=${NIRVANA_TARGET_TRIPLE}"
	"-fsized-deallocation"
	"-fshort-wchar"
)

if(${NIRVANA_TARGET_ARCH} STREQUAL "x64")
	target_compile_options (TestModule PRIVATE "-mlzcnt")
endif()

target_compile_definitions (TestModule PRIVATE NIRVANA_MODULE)
target_link_options(TestModule PRIVATE "LINKER:/nodefaultlib" "LINKER:/noentry")
