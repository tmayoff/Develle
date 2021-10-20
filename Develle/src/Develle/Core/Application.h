#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <Develle/Core/Core.h>
#include <Develle/Core/Window.h>
#include <Develle/Events/Event.h>
#include <Develle/Events/WindowEvent.h>

int main(int argc, char **argv);

namespace Develle {

struct ApplicationCommandLineArgs {
  int Count = 0;
  char **Args = nullptr;

  const char *operator[](int index) const {
    DV_CORE_ASSERT(index < Count);
    return Args[index];
  }
};

class Application {
public:
  Application(const std::string &name = "Develle App",
              ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
  virtual ~Application();

  void OnEvent(Event &e);

  void Close();

  static Application &Get() { return *instance; }

  ApplicationCommandLineArgs GetCommandLineArguments() const {
    return commandLineArgs;
  }

private:
  void Run();
  bool OnWindowClose(WindowCloseEvent &e);

  ApplicationCommandLineArgs commandLineArgs;
  Scope<Window> window;
  bool running = true;
  bool minimized = false;
  float lastFrameTime = 0.0f;

  static Application *instance;
  friend int ::main(int argc, char **argv);
};

// To be defined in the client app
Application *CreateApplication(ApplicationCommandLineArgs args);

} // namespace Develle

#endif // APPLICATION_H_
