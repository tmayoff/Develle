#include <Develle/Core/Application.h>

namespace Develle {

Application *Application::instance = nullptr;

Application::Application(const std::string &name,
                         ApplicationCommandLineArgs args)
    : commandLineArgs(args) {

  DV_CORE_ASSERT(!instance, "Application already exits");

  instance = this;

  window = Window::Create(name);
  window->SetEventCallback(DV_BIND_EVENT_FN(Application::OnEvent));
}

Application::~Application() {}

void Application::Close() { running = false; }

void Application::Run() {
  while (running) {
    if (!minimized) {
      //
    }

    window->OnUpdate();
  }
}

void Application::OnEvent(Event &e) {
  EventDispatcher dispatcher(e);

  dispatcher.Dispatch<WindowCloseEvent>(
      DV_BIND_EVENT_FN(Application::OnWindowClose));
}

bool Application::OnWindowClose(WindowCloseEvent &e) {
  running = false;
  return true;
}

} // namespace Develle
