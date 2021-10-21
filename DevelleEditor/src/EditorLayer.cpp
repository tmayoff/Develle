#include <EditorLayer.h>

#include <imgui.h>

namespace Develle {

EditorLayer::EditorLayer() : Layer("Editor Layer") {}

void EditorLayer::OnAttach() {}

void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate(Timestep ts) {}

void EditorLayer::OnImGuiRender() {
  DV_PROFILE_FUNCTION();

  static bool dockspaceOpen = true;
  static bool optFullscreenPersistant = true;
  bool optFullscreen = optFullscreenPersistant;
  static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

  ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if (optFullscreen) {
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoNavFocus;
  }

  if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
    windowFlags |= ImGuiWindowFlags_NoBackground;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Develle Editor", &dockspaceOpen, windowFlags);
  ImGui::PopStyleVar();
}

void EditorLayer::OnEvent(Event &e) {}

} // namespace Develle
