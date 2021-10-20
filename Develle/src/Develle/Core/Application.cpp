#include <Develle/Core/Application.h>

namespace Develle {

Application *Application::instance = nullptr;

Application::Application(const std::string &name,
                         ApplicationCommandLineArgs args)
    : commandLineArgs(args) {

  DV_PROFILE_FUNCTION();

  DV_CORE_ASSERT(!instance, "Application already exits");

  instance = this;

  window = Window::Create(name);
  window->SetEventCallback(DV_BIND_EVENT_FN(Application::OnEvent));
}

Application::~Application() { DV_PROFILE_FUNCTION(); }

void Application::Close() { running = false; }

void Application::Run() {
  DV_PROFILE_FUNCTION();

  while (running) {
    DV_PROFILE_SCOPE("RunLoop");

    if (!minimized) {
      //
    }

    window->OnUpdate();
  }
}

void Application::OnEvent(Event &e) {
  DV_PROFILE_FUNCTION();

  EventDispatcher dispatcher(e);

  dispatcher.Dispatch<WindowCloseEvent>(
      DV_BIND_EVENT_FN(Application::OnWindowClose));
}

bool Application::OnWindowClose(WindowCloseEvent &e) {
  running = false;
  return true;
}

} // namespace Develle
