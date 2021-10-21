project('Develle', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])

dependencies = []

add_project_arguments('-DDV_DEBUG', language: 'cpp')
if build_machine.system() == 'linux'
  add_project_arguments('-DDV_PLATFORM_LINUX', language: 'cpp')
  SDL2 = dependency('SDL2')
  dependencies += SDL2
elif build_machine.system() == 'windows'
  add_project_arguments('-DDV_PLATFORM_WINDOWS', language: 'cpp')
endif

spdlog = subproject('spdlog')
spdlog_dep = spdlog.get_variable('spdlog_dep')

dependencies += [spdlog_dep]


subdir('Develle')
subdir('DevelleEditor')
subdir('Sandbox')
