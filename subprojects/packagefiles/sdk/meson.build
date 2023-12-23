project ('sdk', 'cpp')

sdk_dep = declare_dependency (
	include_directories: include_directories ('include')
)

nidl2cpp = find_program ('nidl2cpp', 'bin')

meson.override_dependency('sdk', sdk_dep)