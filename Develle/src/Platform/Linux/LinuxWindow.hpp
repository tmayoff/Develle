#ifndef LINUXWINDOW_H_
#define LINUXWINDOW_H_

#include <SDL2/SDL.h>

#include <Develle/Core/Window.hpp>
#include <Develle/Renderer/GraphicsContext.hpp>

namespace Develle {

class LinuxWindow : public Window {
 public:
  LinuxWindow(const WindowProps &props);
  virtual ~LinuxWindow();

  void OnUpdate() override;

  uint32_t GetWidth() const override { return data.Width; }
  uint32_t GetHeight() const override { return data.Height; }

  void SetEventCallback(const EventCallbackFn &callback) override { data.EventCallback = callback; }
  void SetVSync(bool enabled) override;
  bool IsVSync() const override;

  virtual void *GetNativeWindow() const override { return window; }
  GraphicsContext *GetGraphicsContext() const override {
    // HACK(tyler)
    return context.get();
  }

 private:
  virtual void Init(const WindowProps &props);
  virtual void Shutdown();

  SDL_Window *window;
  Scope<GraphicsContext> context;

  struct WindowData {
    std::string Title;
    uint32_t Width, Height;
    bool VSync;

    EventCallbackFn EventCallback;
  };

  WindowData data;
};

}  // namespace Develle
#endif  // LINUXWINDOW_H_
