cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_SCAN_FOR_MODULES 0)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

file(TO_CMAKE_PATH $ENV{NIRVANA_SDK} NIRVANA_SDK_DIR)
set(NIRVANA_TOOLS ${NIRVANA_SDK_DIR}/bin/x64)

#set(CMAKE_CXX_COMPILER ${NIRVANA_TOOLS}/clang++.exe)
#set(CMAKE_CXX_COMPILER_WORKS ON)
#set(CMAKE_C_COMPILER ${NIRVANA_TOOLS}/clang.exe)
#set(CMAKE_C_COMPILER_WORKS ON)
#set(CMAKE_ASM_COMPILER ${NIRVANA_TOOLS}/clang.exe)
#set(CMAKE_RC_COMPILER ${NIRVANA_TOOLS}/llvm-rc.exe)

set(NIDL2CPP ${NIRVANA_TOOLS}/nidl2cpp.exe)

set(NIRVANA_TARGET_ARCH "x64")

if(${NIRVANA_TARGET_ARCH} STREQUAL "")
message(FATAL_ERROR "NIRVANA_TARGET_ARCH must be specified")
endif()

set(NIRVANA_TARGET_TRIPLE "x86_64-pc-win32")

set(NIRVANA_LIB_DIR ${NIRVANA_SDK_DIR}/lib/${NIRVANA_TARGET_ARCH})

function(idl_generate)

	set(options)
	set(one_args OUT_VAR)
	set(multi_args IDL_FILES OPTIONS)
	cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_args}" "${multi_args}")

	foreach(f IN LISTS arg_IDL_FILES)
		list(APPEND idl_idl ${f})
		cmake_path(GET f STEM name)
		list(APPEND idl_h ${CMAKE_CURRENT_BINARY_DIR}/IDL/${name}.h)
		list(APPEND idl_h ${CMAKE_CURRENT_BINARY_DIR}/IDL/${name}_s.h)
		list(APPEND idl_cpp ${CMAKE_CURRENT_BINARY_DIR}/IDL/${name}.cpp)
	endforeach()

	add_custom_command(
		OUTPUT ${idl_h} ${idl_cpp}
		COMMAND ${NIDL2CPP} ARGS ${IDL_OPTIONS} ${arg_OPTIONS} -I ${NIRVANA_SDK_DIR}/include -out ${CMAKE_CURRENT_BINARY_DIR}/IDL ${idl_idl}
		DEPENDS ${idl_idl}
		COMMENT "Compile IDL..."
		VERBATIM
		BYPRODUCTS
	)

	set(${arg_OUT_VAR} ${idl_cpp} PARENT_SCOPE)

endfunction()

set (NIRVANA_INCLUDES
	${NIRVANA_SDK_DIR}/include/c++/v1
	${NIRVANA_SDK_DIR}/include/clang
	${NIRVANA_SDK_DIR}/include
)

set (NIRVANA_LIBS
	${NIRVANA_LIB_DIR}/$<CONFIG>/libc++.lib
	${NIRVANA_LIB_DIR}/$<CONFIG>/libc++experimental.lib
	${NIRVANA_LIB_DIR}/$<CONFIG>/crtl.lib
	${NIRVANA_LIB_DIR}/$<CONFIG>/libm.lib
	${NIRVANA_LIB_DIR}/$<CONFIG>/nirvana.lib
	${NIRVANA_LIB_DIR}/$<CONFIG>/coreimport.lib
)

project (TestORB CXX)

add_subdirectory (idl)
add_subdirectory (module)

