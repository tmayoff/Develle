project('Develle', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

dependencies = []

add_project_arguments('-DDV_DEBUG', language: 'cpp')
add_project_arguments('-DDV_ENABLE_ASSERTS', language: 'cpp')
if build_machine.system() == 'linux'
  add_project_arguments('-DDV_PLATFORM_LINUX', language: 'cpp')
  SDL2 = dependency('SDL2')
  dependencies += SDL2
elif build_machine.system() == 'windows'
  add_project_arguments('-DDV_PLATFORM_WINDOWS', language: 'cpp')
endif

spdlog = subproject('spdlog')
spdlog_dep = spdlog.get_variable('spdlog_dep')

imgui = subproject('imgui')
imgui_dep = imgui.get_variable('imgui_dep')

glm = subproject('glm')
glm_dep = glm.get_variable('glm_dep')

subdir('external')

dependencies += [spdlog_dep, entt_dep, imgui_dep, glad_dep, glm_dep, stbi_dep]

# Targets
subdir('Develle')
subdir('DevelleEditor')
subdir('Sandbox')
