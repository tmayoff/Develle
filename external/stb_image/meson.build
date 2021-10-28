stbi_inc = include_directories('include')

stbi_lib = static_library('stb_image', include_directories: stbi_inc)

stbi_dep = declare_dependency(include_directories: stbi_inc, link_with: stbi_lib)
