#include "LinuxWindow.hpp"

#include <Develle/Events/ApplicationEvent.hpp>
#include <Develle/Events/KeyEvent.hpp>
#include <Develle/Events/MouseEvent.hpp>

namespace Develle {

LinuxWindow::LinuxWindow(const WindowProps &props) : window(nullptr) {
  DV_PROFILE_FUNCTION();  // NOLINT
  Init(props);
}

LinuxWindow::~LinuxWindow() {
  DV_PROFILE_FUNCTION();  // NOLINT
  Shutdown();
}

void LinuxWindow::Shutdown() {
  DV_PROFILE_FUNCTION();  // NOLINT
  SDL_Quit();
}

void LinuxWindow::SetVSync(bool enabled) {
  DV_PROFILE_FUNCTION();  // NOLINT
  SDL_GL_SetSwapInterval(enabled);
}

bool LinuxWindow::IsVSync() const { return SDL_GL_GetSwapInterval(); }

void LinuxWindow::Init(const WindowProps &props) {
  DV_PROFILE_FUNCTION();  // NOLINT

  data.Title = props.Title;
  data.Width = props.Width;
  data.Height = props.Height;

  {
    DV_PROFILE_SCOPE("SDL_Init");  // NOLINT
    SDL_Init(SDL_INIT_VIDEO);
  }

  {
    DV_PROFILE_SCOPE("SDL_CreateWindow");  // NOLINT
    window = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              (int)props.Width, (int)props.Height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  }

  context = GraphicsContext::Create(window);
  context->Init();
}

void LinuxWindow::OnUpdate() {
  DV_PROFILE_FUNCTION();  // NOLINT

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    DV_PROFILE_SCOPE("Poll Event");  // NOLINT
    switch (e.type) {
      case SDL_WINDOWEVENT: {
        switch (e.window.event) {
          case SDL_WINDOWEVENT_RESIZED: {
            data.Width = e.window.data1;
            data.Height = e.window.data2;

            WindowResizeEvent event(e.window.data1, e.window.data2);
            data.EventCallback(event);
            break;
          }
          case SDL_WINDOWEVENT_CLOSE: {
            WindowCloseEvent event;
            data.EventCallback(event);
            break;
          }
        }

        break;
      }
      case SDL_TEXTINPUT: {
        KeyTypedEvent event(e.text.text);  // NOLINT
        data.EventCallback(event);
        break;
      }
      case SDL_KEYUP: {
        KeyReleasedEvent event(e.key.keysym.scancode);
        data.EventCallback(event);
        break;
      }
      case SDL_KEYDOWN: {
        KeyPressedEvent event(e.key.keysym.scancode, e.key.repeat);
        data.EventCallback(event);
        break;
      }
      case SDL_MOUSEMOTION: {
        MouseMovedEvent event((float)e.motion.x, (float)e.motion.y);
        data.EventCallback(event);
        break;
      }
      case SDL_MOUSEWHEEL: {
        MouseScrolledEvent event((float)e.wheel.x, (float)e.wheel.y);
        data.EventCallback(event);
        break;
      }
      case SDL_MOUSEBUTTONUP: {
        MouseCode mouseButton = (MouseCode)0;
        switch (e.button.button) {
          case SDL_BUTTON_LEFT:
            mouseButton = MouseCode::ButtonLeft;
            break;
          case SDL_BUTTON_MIDDLE:
            mouseButton = MouseCode::ButtonMiddle;
            break;
          case SDL_BUTTON_RIGHT:
            mouseButton = MouseCode::ButtonRight;
            break;
        }
        MouseButtonReleasedEvent event(mouseButton);
        data.EventCallback(event);
        break;
      }
      case SDL_MOUSEBUTTONDOWN: {
        MouseCode mouseButton = (MouseCode)0;
        switch (e.button.button) {
          case SDL_BUTTON_LEFT:
            mouseButton = MouseCode::ButtonLeft;
            break;
          case SDL_BUTTON_MIDDLE:
            mouseButton = MouseCode::ButtonMiddle;
            break;
          case SDL_BUTTON_RIGHT:
            mouseButton = MouseCode::ButtonRight;
            break;
        }

        MouseButtonPressedEvent event(mouseButton);
        data.EventCallback(event);
        break;
      }
      case SDL_EventType::SDL_QUIT:
        WindowCloseEvent event;
        data.EventCallback(event);
    }
  }

  {
    DV_PROFILE_SCOPE("Swap window");  // NOLINT
    SDL_GL_SwapWindow(window);
  }
}

}  // namespace Develle
