#include "Application.hpp"

#include <SDL2/SDL.h>

#include <Develle/Renderer/Renderer.hpp>

namespace Develle {

Application *Application::instance = nullptr;

Application::Application(const std::string &name, ApplicationCommandLineArgs args)
    : commandLineArgs(args) {
  DV_PROFILE_FUNCTION();

  DV_CORE_ASSERT(!instance, "Application already exits");

  instance = this;

  window = Window::Create(name);
  window->SetEventCallback(DV_BIND_EVENT_FN(Application::OnEvent));

  Renderer::Init();

  imGuiLayer = new ImGuiLayer();
  PushOverlay(imGuiLayer);
}

Application::~Application() { DV_PROFILE_FUNCTION(); }

void Application::PushLayer(Layer *layer) {
  DV_PROFILE_FUNCTION();
  layerStack.PushOverlay(layer);
  layer->OnAttach();
}

void Application::PushOverlay(Layer *layer) {
  DV_PROFILE_FUNCTION();
  layerStack.PushOverlay(layer);
  layer->OnAttach();
}

void Application::Close() { running = false; }

void Application::Run() {
  DV_PROFILE_FUNCTION();

  while (running) {
    DV_PROFILE_SCOPE("RunLoop");

    float timeMS = static_cast<float>(SDL_GetTicks());
    Timestep delta = timeMS - lastFrameTimeMS;
    lastFrameTimeMS = timeMS;

    if (!minimized) {
      {
        DV_PROFILE_SCOPE("LayerStack OnUpdate");

        for (Layer *layer : layerStack) layer->OnUpdate(delta);
      }

      imGuiLayer->Begin();
      {
        DV_PROFILE_SCOPE("LayerStack OnImGuiRender");

        for (Layer *layer : layerStack) layer->OnImGuiRender();
      }
      imGuiLayer->End();
    }

    window->OnUpdate();
  }
}

void Application::OnEvent(Event &e) {
  DV_PROFILE_FUNCTION();

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
  DV_PROFILE_FUNCTION();

  // if (e.GetWidth())

  return false;
}

}  // namespace Develle
