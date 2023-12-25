gen_files = sdk.get_variable ('idl_full').process(idl_files)

mod_files = files (
	'I1_factory_dynamic.cpp',
	'I1_factory_portable.cpp',
	'I1_factory_sysdomain.cpp',
	'I1_factory_tied.cpp',
	'I1_static.cpp',
	'I2_factory_dynamic.cpp',
	'I2_factory_sysdomain.cpp',
	'I3_factory_dynamic.cpp',
	'I3_factory_portable.cpp',
	'I3_factory_tied.cpp',
	'I3_static.cpp',
	'ImplI1.cpp',
	'ImplI2.cpp',
	'ImplV1.cpp',
	'ImplV2.cpp',
	'ImplV3.cpp',
	'PingPongImpl.cpp'
)

shared_library ('TestModule',
	gen_files, mod_files,
	name_suffix: 'olf',
	dependencies: sdk.get_variable ('sdk_deps')['x64'],
	include_directories: '../include'
	)
