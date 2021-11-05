project('tracy')

add_project_arguments('-DTRACY_ENABLE', language: 'cpp')

inc = include_directories('.')
lib_tracy = static_library('tracy', 
	['TracyClient.cpp', 'client/TracyAlloc.cpp', 'client/TracyProfiler.cpp', 'client/tracy_rpmalloc.cpp'], 
	include_directories: inc)

tracy_dep = declare_dependency(include_directories : inc, link_with: lib_tracy)

meson.override_dependency('tracy', tracy_dep)
