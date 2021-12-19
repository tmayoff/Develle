#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include <Develle/Events/ApplicationEvent.hpp>
#include <Develle/Events/Event.hpp>
#include <Develle/ImGui/ImGuiLayer.hpp>

#include "Core.hpp"
#include "LayerStack.hpp"
#include "Window.hpp"

int main(int argc, char **argv);

namespace Develle {

struct ApplicationCommandLineArgs {
  int Count = 0;
  char **Args = nullptr;

  const char *operator[](int index) const {
    DV_CORE_ASSERT(index < Count, "Index out of range");
    return Args[index];
  }
};

class Application {
 public:
  Application(const std::string &name = "Develle App",
              ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
  virtual ~Application();

  void OnEvent(Event &e);

  void PushLayer(Layer *layer);
  void PushOverlay(Layer *layer);

  Window &GetWindow() { return *window; }

  void Close();

  ImGuiLayer *GetImGuiLayer() { return imGuiLayer; }

  static Application &Get() { return *instance; }

  ApplicationCommandLineArgs GetCommandLineArguments() const { return commandLineArgs; }

 private:
  void Run();
  bool OnWindowClose(WindowCloseEvent &e);
  bool OnWindowResize(WindowResizeEvent &e);

  ApplicationCommandLineArgs commandLineArgs;
  Scope<Window> window;
  LayerStack layerStack;
  ImGuiLayer *imGuiLayer;
  bool running = true;
  bool minimized = false;
  float lastFrameTimeMS = 0.0f;

  static Application *instance;
  friend int ::main(int argc, char **argv);
};

// To be defined in the client app
Application *CreateApplication(ApplicationCommandLineArgs args);

}  // namespace Develle

#endif  // APPLICATION_HPP_
