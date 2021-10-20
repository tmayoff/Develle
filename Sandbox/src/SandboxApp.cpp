#include <Develle.h>
#include <Develle/Core/EntryPoint.h>

class Sandbox : public Develle::Application {
public:
  Sandbox(Develle::ApplicationCommandLineArgs) {}

  ~Sandbox() {}
};

Develle::Application *
Develle::CreateApplication(Develle::ApplicationCommandLineArgs args) {
  return new Sandbox(args);
}
