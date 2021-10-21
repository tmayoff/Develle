#include <Develle/ImGui/ImGuiLayer.h>

#include <Develle/Core/Application.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <SDL2/SDL.h>

namespace Develle {

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

void ImGuiLayer::OnAttach() {
  DV_PROFILE_FUNCTION();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  float fontSize = 18.0f;

  Application &app = Application::Get();
  SDL_Window *window =
      static_cast<SDL_Window *>(app.GetWindow().GetNativeWindow());

  ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
  ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach() {
  DV_PROFILE_FUNCTION();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::OnEvent(Event &e) {
  if (blockEvents) {
    ImGuiIO &io = ImGui::GetIO();
    e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
    e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
  }
}

void ImGuiLayer::Begin() {
  DV_PROFILE_FUNCTION();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayer::End() {
  DV_PROFILE_FUNCTION();

  ImGuiIO &io = ImGui::GetIO();
  Application &app = Application::Get();
  io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(),
                          (float)app.GetWindow().GetHeight());

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
  }
}

} // namespace Develle
