#ifndef EDITORLAYER_H_
#define EDITORLAYER_H_

#include <Develle.h>

#include <Panels/SceneHierarchyPanel.h>

namespace Develle {

class EditorLayer : public Layer {
public:
  EditorLayer();
  ~EditorLayer() = default;

  void OnAttach() override;
  void OnDetach() override;

  void OnUpdate(Timestep ts) override;
  void OnImGuiRender() override;
  void OnEvent(Event &e) override;

  SceneHierarchyPanel sceneHierarchyPanel;
  // ContentBrowserPanel contentBrowserPanel;
private:
  Ref<Framebuffer> framebuffer;
};

} // namespace Develle
#endif // EDITORLAYER_H_
