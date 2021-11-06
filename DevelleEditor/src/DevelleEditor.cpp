#include <Develle.h>
#include <Develle/Core/EntryPoint.h>

#include <EditorLayer.hpp>

namespace Develle {

class DevelleEditor : public Application {
 public:
  DevelleEditor(ApplicationCommandLineArgs args) : Application("Develle Editor", args) {
    PushLayer(new EditorLayer());
  }

  ~DevelleEditor() {}
};

Application *CreateApplication(ApplicationCommandLineArgs args) { return new DevelleEditor(args); }

}  // namespace Develle
