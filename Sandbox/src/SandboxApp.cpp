#include <Develle.hpp>
#include <Develle/Core/EntryPoint.hpp>
#include <Sandbox2D.hpp>

class Sandbox : public Develle::Application {
 public:
  Sandbox(Develle::ApplicationCommandLineArgs) { PushLayer(new Sandbox2D()); }

  ~Sandbox() {}
};

Develle::Application* Develle::CreateApplication(Develle::ApplicationCommandLineArgs args) {
  return new Sandbox(args);
}
