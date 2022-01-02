#include "LinuxWindow.hpp"

#include <Develle/Events/ApplicationEvent.hpp>
#include <Develle/Events/KeyEvent.hpp>
#include <Develle/Events/MouseEvent.hpp>
#include <Develle/Renderer/RendererAPI.hpp>

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

  {
    DV_PROFILE_SCOPE("SDL_Init");
    SDL_Init(SDL_INIT_VIDEO);
  }

  int windowFlags = SDL_WINDOW_SHOWN;
  switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::OpenGL:
      windowFlags |= SDL_WINDOW_OPENGL;
      break;
    case RendererAPI::API::Vulkan:
      windowFlags |= SDL_WINDOW_VULKAN;
      break;
    default:
      DV_CORE_ERROR("Unknown RendererAPI");
      break;
  }

  {
    DV_PROFILE_SCOPE("SDL_CreateWindow");
    window = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              props.Width, props.Height, windowFlags | SDL_WINDOW_RESIZABLE);
  }

  context = GraphicsContext::Create(window);
  context->Init();
}

void LinuxWindow::OnUpdate() {
  DV_PROFILE_FUNCTION();

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    DV_PROFILE_SCOPE("Poll Event");
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
        KeyTypedEvent event(e.text.text);
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
        MouseMovedEvent event(e.motion.x, e.motion.y);
        data.EventCallback(event);
        break;
      }
      case SDL_MOUSEWHEEL: {
        MouseScrolledEvent event(e.wheel.x, e.wheel.y);
        data.EventCallback(event);
        break;
      }
      case SDL_MOUSEBUTTONUP: {
        MouseButtonReleasedEvent event(e.button.button);
        data.EventCallback(event);
        break;
      }
      case SDL_MOUSEBUTTONDOWN: {
        MouseButtonPressedEvent event(e.button.button);
        data.EventCallback(event);
        break;
      }
      case SDL_EventType::SDL_QUIT:
        WindowCloseEvent event;
        data.EventCallback(event);
    }
  }

  {
    DV_PROFILE_SCOPE("Swap window");
    SDL_GL_SwapWindow(window);
  }
}

}  // namespace Develle
