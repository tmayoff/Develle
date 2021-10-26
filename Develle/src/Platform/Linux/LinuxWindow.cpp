#include <Develle/Events/ApplicationEvent.hpp>
#include <Platform/Linux/LinuxWindow.h>

namespace Develle {

LinuxWindow::LinuxWindow(const WindowProps &props) {
  DV_PROFILE_FUNCTION();
  Init(props);
}

LinuxWindow::~LinuxWindow() {
  DV_PROFILE_FUNCTION();
  Shutdown();
}

void LinuxWindow::Shutdown() {
  DV_PROFILE_FUNCTION();
  SDL_Quit();
}

void LinuxWindow::SetVSync(bool enabled) {
  DV_PROFILE_FUNCTION();
  SDL_GL_SetSwapInterval(enabled);
}

bool LinuxWindow::IsVSync() const { return SDL_GL_GetSwapInterval(); }

void LinuxWindow::Init(const WindowProps &props) {
  DV_PROFILE_FUNCTION();

  data.Title = props.Title;
  data.Width = props.Width;
  data.Height = props.Height;

  SDL_Init(SDL_INIT_VIDEO);

  {
    DV_PROFILE_SCOPE("SDL_CreateWindow");
    window = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, props.Width, props.Height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  }

  context = GraphicsContext::Create(window);
  context->Init();
}

void LinuxWindow::OnUpdate() {
  DV_PROFILE_FUNCTION();

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EventType::SDL_QUIT:
      WindowCloseEvent event;
      data.EventCallback(event);
    }
  }

  SDL_GL_SwapWindow(window);
}

} // namespace Develle
