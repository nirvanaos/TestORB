gen_files = idl_full.process(idl_files)

mod_files = files (
	'I1_factory_dynamic.cpp',
	'I1_factory_portable.cpp',
	'I1_factory_tied.cpp',
	'I1_static.cpp'

)

shared_module ('TestModule', 'cpp', gen_files, mod_files,
	dependencies: sdk_dep
	)
