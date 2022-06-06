#include "Application.hpp"

#include <SDL2/SDL.h>

#include <Develle/Renderer/Renderer.hpp>

namespace Develle {

Application *Application::instance = nullptr;

Application::Application(const std::string &name, ApplicationCommandLineArgs args, RenderMode mode)
    : commandLineArgs(args) {
  DV_PROFILE_FUNCTION();  // NOLINT

  DV_CORE_ASSERT(!instance, "Application already exits");

  instance = this;

  window = Window::Create(name);
  window->SetEventCallback(DV_BIND_EVENT_FN(Application::OnEvent));

  Renderer::Init(mode);

  imGuiLayer = new ImGuiLayer();
  PushOverlay(imGuiLayer);
}

Application::~Application() {
  DV_PROFILE_FUNCTION();  // NOLINT
}

void Application::PushLayer(Layer *layer) {
  DV_PROFILE_FUNCTION();  // NOLINT
  layerStack.PushOverlay(layer);
  layer->OnAttach();
}

void Application::PushOverlay(Layer *layer) {
  DV_PROFILE_FUNCTION();  // NOLINT
  layerStack.PushOverlay(layer);
  layer->OnAttach();
}

void Application::Close() { running = false; }

void Application::Run() {
  DV_PROFILE_FUNCTION();  // NOLINT

  while (running) {
    DV_PROFILE_SCOPE("RunLoop");  // NOLINT

    float timeMS = static_cast<float>(SDL_GetTicks());
    Timestep delta = timeMS - lastFrameTimeMS;
    lastFrameTimeMS = timeMS;

    if (!minimized) {
      {
        DV_PROFILE_SCOPE("LayerStack OnUpdate");  // NOLINT

        for (Layer *layer : layerStack) layer->OnUpdate(delta);
      }

      imGuiLayer->Begin();
      {
        DV_PROFILE_SCOPE("LayerStack OnImGuiRender");  // NOLINT

        for (Layer *layer : layerStack) layer->OnImGuiRender();
      }
      imGuiLayer->End();
    }

    window->OnUpdate();
    DV_PROFILE_FRAME("Main");
  }
}

void Application::OnEvent(Event &e) {
  DV_PROFILE_FUNCTION();  // NOLINT

  EventDispatcher dispatcher(e);

  dispatcher.Dispatch<WindowCloseEvent>(DV_BIND_EVENT_FN(Application::OnWindowClose));

  for (auto it = layerStack.rbegin(); it != layerStack.rend(); ++it) {
    if (e.Handled) break;
    (*it)->OnEvent(e);
  }
}

bool Application::OnWindowClose(WindowCloseEvent &) {
  running = false;
  return true;
}

bool Application::OnWindowResize(WindowResizeEvent &e) {
  DV_PROFILE_FUNCTION();  // NOLINT

  // if (e.GetWidth())

  return false;
}

}  // namespace Develle
