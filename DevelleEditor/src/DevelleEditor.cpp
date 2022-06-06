#include <Develle.hpp>
#include <Develle/Core/EntryPoint.hpp>
#include <EditorLayer.hpp>

namespace Develle {

class DevelleEditor : public Application {
 public:
  DevelleEditor(ApplicationCommandLineArgs args)
      : Application("Develle Editor", args, RenderMode::Render2D) {
    PushLayer(new EditorLayer());
  }

  ~DevelleEditor() {}
};

Application *CreateApplication(ApplicationCommandLineArgs args) { return new DevelleEditor(args); }

}  // namespace Develle
