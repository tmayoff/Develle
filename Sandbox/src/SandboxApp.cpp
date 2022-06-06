#include <Develle.hpp>
#include <Develle/Core/EntryPoint.hpp>

#include "Sandbox2D.hpp"
#include "Sandbox3D.hpp"

class Sandbox : public Develle::Application {
 public:
  Sandbox(Develle::ApplicationCommandLineArgs args)
      : Develle::Application("", args, Develle::RenderMode::Render3D) {
    // PushLayer(new Sandbox2D());
    PushLayer(new Sandbox3D());
  }

  ~Sandbox() {}
};

Develle::Application* Develle::CreateApplication(Develle::ApplicationCommandLineArgs args) {
  return new Sandbox(args);
}
