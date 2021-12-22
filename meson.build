project('Develle', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20', 'b_sanitize=address'])

# Options
enable_profiling = get_option('profiling')

dependencies = []

shaderc = dependency('shaderc')

if enable_profiling
  add_project_arguments('-DDV_PROFILE', language: 'cpp')
endif
add_project_arguments('-DDV_DEBUG', language: 'cpp')
add_project_arguments('-DDV_ENABLE_ASSERTS', language: 'cpp')
if build_machine.system() == 'linux'
  add_project_arguments('-DDV_PLATFORM_LINUX', language: 'cpp')
  SDL2 = dependency('SDL2')
  dependencies += SDL2
elif build_machine.system() == 'windows'
  add_project_arguments('-DDV_PLATFORM_WINDOWS', language: 'cpp')
endif

assets_root = meson.current_source_dir() + '/Develle/assets'
add_project_arguments('-DASSETS_ROOT="' + assets_root + '"', language: 'cpp')

spdlog = subproject('spdlog')
spdlog_dep = spdlog.get_variable('spdlog_dep')

imgui = subproject('imgui')
imgui_dep = imgui.get_variable('imgui_dep')

glm = subproject('glm')
glm_dep = glm.get_variable('glm_dep')

cmake = import('cmake')
tracy_opt = cmake.subproject_options()
tracy_opt.add_cmake_defines({'BUILD_SHARED_LIBS': true})
if enable_profiling
  tracy_opt.add_cmake_defines({'ENABLE_TRACY': true})
endif
tracy = cmake.subproject('tracy', options: tracy_opt)
tracy_dep = tracy.dependency('TracyClient')

spirv_cross_opt = cmake.subproject_options()
spirv_cross_opt.add_cmake_defines({'SPIRV_CROSS_STATIC': true})
spirv_cross_opt.add_cmake_defines({'SPIRV_CROSS_FORCE_PIC': true})
spirv_cross = cmake.subproject('spirv_cross', options: spirv_cross_opt)
spirv_cross_core_dep = spirv_cross.dependency('spirv-cross-core')
spirv_cross_cpp_dep = spirv_cross.dependency('spirv-cross-cpp')
spirv_cross_glsl_dep = spirv_cross.dependency('spirv-cross-glsl')

yaml_opt = cmake.subproject_options()
yaml_opt.add_cmake_defines({'YAML_BUILD_SHARED_LIBS': true})
yaml = cmake.subproject('yaml-cpp', options: yaml_opt)
yaml_dep = yaml.dependency('yaml-cpp')

pfd = cmake.subproject('pfd')
pfd_dep = pfd.dependency('portable-file-dialogs')

subdir('external')

dependencies += [shaderc, spdlog_dep, entt_dep, imgui_dep, glad_dep, glm_dep, stbi_dep,
                tracy_dep, yaml_dep, pfd_dep,
                spirv_cross_core_dep, spirv_cross_cpp_dep, spirv_cross_glsl_dep]

# Targets
subdir('Develle')
subdir('DevelleEditor')
subdir('Sandbox')
