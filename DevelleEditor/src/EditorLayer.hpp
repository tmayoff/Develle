#ifndef EDITORLAYER_H_
#define EDITORLAYER_H_

#include <Develle.h>
#include <Develle/Scene/Scene.h>

#include <Panels/SceneHierarchyPanel.hpp>

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

 private:
  Ref<Scene> activeScene;

  // Entity
  Entity camera;
  Entity hoveredEntity;

  EditorCamera editorCamera;

  SceneHierarchyPanel sceneHierarchyPanel;
  // ContentBrowserPanel contentBrowserPanel;
  Ref<Framebuffer> framebuffer;

  bool viewportFocused = false, viewportHovered = false;
  glm::vec2 viewportSize = {0.0f, 0.0f};
  glm::vec2 viewportBounds[2];
};

}  // namespace Develle
#endif  // EDITORLAYER_H_
