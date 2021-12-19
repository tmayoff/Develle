project('tracy')

add_project_arguments('-DTRACY_ENABLE', language: 'cpp')

cpp = meson.get_compiler('cpp')
dl = cpp.find_library('dl')

tracy_inc = include_directories('.')
tracy_lib = static_library('tracy', 
	['TracyClient.cpp', 'client/TracyAlloc.cpp', 'client/TracyProfiler.cpp', 'client/tracy_rpmalloc.cpp'], 
	dependencies: dl,
	include_directories: tracy_inc)

tracy_dep = declare_dependency(include_directories: tracy_inc, link_with: tracy_lib)
