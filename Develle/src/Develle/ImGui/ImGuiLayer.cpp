#include "ImGuiLayer.hpp"

#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <Develle/Core/Application.hpp>

namespace Develle {

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

void ImGuiLayer::OnAttach() {
  DV_PROFILE_FUNCTION();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  float fontSize = 18.0f;

  Application &app = Application::Get();
  SDL_Window *window = static_cast<SDL_Window *>(app.GetWindow().GetNativeWindow());

  ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
  ImGui_ImplOpenGL3_Init("#version 410");

  // KeyMap
  io.KeyMap[ImGuiKey_Tab] = Key::TAB;
  io.KeyMap[ImGuiKey_RightArrow] = Key::RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = Key::UP;
  io.KeyMap[ImGuiKey_DownArrow] = Key::DOWN;
  io.KeyMap[ImGuiKey_PageUp] = Key::PAGEUP;
  io.KeyMap[ImGuiKey_PageDown] = Key::PAGEDOWN;
  io.KeyMap[ImGuiKey_Home] = Key::HOME;
  io.KeyMap[ImGuiKey_End] = Key::END;
  io.KeyMap[ImGuiKey_Insert] = Key::INSERT;
  io.KeyMap[ImGuiKey_Delete] = Key::DELETE;
  io.KeyMap[ImGuiKey_Backspace] = Key::BACKSPACE;
  io.KeyMap[ImGuiKey_Space] = Key::SPACE;
  io.KeyMap[ImGuiKey_Enter] = Key::RETURN;
  io.KeyMap[ImGuiKey_Escape] = Key::ESCAPE;
  io.KeyMap[ImGuiKey_KeyPadEnter] = Key::KP_ENTER;
  io.KeyMap[ImGuiKey_A] = Key::A;
  io.KeyMap[ImGuiKey_C] = Key::C;
  io.KeyMap[ImGuiKey_V] = Key::V;
  io.KeyMap[ImGuiKey_X] = Key::X;
  io.KeyMap[ImGuiKey_Y] = Key::Y;
  io.KeyMap[ImGuiKey_Z] = Key::Z;
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

  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<KeyPressedEvent>(DV_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
  dispatcher.Dispatch<KeyReleasedEvent>(DV_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
  dispatcher.Dispatch<KeyTypedEvent>(DV_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
  dispatcher.Dispatch<MouseButtonPressedEvent>(
      DV_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
  dispatcher.Dispatch<MouseButtonReleasedEvent>(
      DV_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
  dispatcher.Dispatch<MouseScrolledEvent>(DV_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
  // dispatcher.Dispatch<MouseMovedEvent>(
  //     DV_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
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
  io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    SDL_Window *backupWindow = SDL_GL_GetCurrentWindow();
    SDL_GLContext backupContext = SDL_GL_GetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    SDL_GL_MakeCurrent(backupWindow, backupContext);
  }
}

bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  io.KeysDown[e.GetKeyCode()] = true;
  io.KeyCtrl = io.KeysDown[Key::LCTRL] || io.KeysDown[Key::RCTRL];
  io.KeyAlt = io.KeysDown[Key::LALT] || io.KeysDown[Key::RALT];
  io.KeyShift = io.KeysDown[Key::LSHIFT] || io.KeysDown[Key::RSHIFT];
  return false;
}

bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  io.KeysDown[e.GetKeyCode()] = false;
  io.KeyCtrl = io.KeysDown[Key::LCTRL] || io.KeysDown[Key::RCTRL];
  io.KeyAlt = io.KeysDown[Key::LALT] || io.KeysDown[Key::RALT];
  io.KeyShift = io.KeysDown[Key::LSHIFT] || io.KeysDown[Key::RSHIFT];
  return false;
}

bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  io.AddInputCharactersUTF8(e.GetKey());
  return false;
}

bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  switch (e.GetMouseButton()) {
    case Mouse::ButtonLeft:
      io.MouseDown[0] = true;
      break;
    case Mouse::ButtonRight:
      io.MouseDown[1] = true;
      break;
    case Mouse::ButtonMiddle:
      io.MouseDown[2] = true;
      break;
  }

  return false;
}

bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  switch (e.GetMouseButton()) {
    case Mouse::ButtonLeft:
      io.MouseDown[0] = false;
      break;
    case Mouse::ButtonRight:
      io.MouseDown[1] = false;
      break;
    case Mouse::ButtonMiddle:
      io.MouseDown[2] = false;
      break;
  }

  return false;
}

bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  io.MouseWheelH = e.GetXOffset();
  io.MouseWheel = e.GetYOffset();
  return false;
}

}  // namespace Develle
