cpp = meson.get_compiler('cpp')

dl = cpp.find_library('dl')

glad_inc = include_directories('include')
glad_lib = static_library('glad', 'src/glad.cpp', 
	dependencies: dl, 
	include_directories:  glad_inc)

glad_dep = declare_dependency(
	include_directories: glad_inc, 
	link_with: glad_lib)
