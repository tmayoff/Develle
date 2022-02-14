project('ImGuizmo', 'cpp')

sources = files('ImGuizmo.cpp', 'ImCurveEdit.cpp', 'ImGradient.cpp')

imgui = subproject('imgui')
imgui_dep = imgui.get_variable('imgui_dep')

deps = [imgui_dep]

imguizmo_lib = library('ImGuizmo', sources: sources, dependencies: deps, include_directories: '.')
imguizmo_dep = declare_dependency(dependencies: deps, include_directories: '.', link_with: imguizmo_lib)
