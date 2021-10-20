#include <Develle/Events/WindowEvent.h>
#include <Platform/Linux/LinuxWindow.h>

namespace Develle {

LinuxWindow::LinuxWindow(const WindowProps &props) { Init(props); }

LinuxWindow::~LinuxWindow() { Shutdown(); }

void LinuxWindow::Shutdown() { SDL_Quit(); }

void LinuxWindow::SetVSync(bool enabled) {}

bool LinuxWindow::IsVSync() const {}

void LinuxWindow::Init(const WindowProps &props) {
  data.Title = props.Title;
  data.Width = props.Width;
  data.Height = props.Height;

  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, props.Width, props.Height,
                            SDL_WINDOW_SHOWN);

  // TODO(tyer) Init Graphics Context
}

void LinuxWindow::OnUpdate() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EventType::SDL_QUIT:
      WindowCloseEvent event;
      data.EventCallback(event);
    }
  }
}

} // namespace Develle
