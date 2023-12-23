project('TestORB', 'cpp',
	default_options: [
		'cpp_std=c++17',
		'b_ndebug=if-release', 
		'default_library=static',
		'prefer_static=true',
		'cpp_debugstl=true',
		'cpp_link_args=-static'
	]
)

sdk_dep = dependency ('sdk')

idl_full = generator (nidl2cpp,
	output: [ '@BASENAME@.h', '@BASENAME@_s.h', '@BASENAME@.cpp', '@BASENAME@_p.cpp' ],
	arguments: [ '@INPUT@', '-out', '@BUILD_DIR@' ]
	)

idl_client = generator (nidl2cpp,
	output: [ '@BASENAME@.h', '@BASENAME@.cpp' ],
	arguments: [ '@INPUT@', '-client', '-out', '@BUILD_DIR@' ]
	)

subdir ('module')
