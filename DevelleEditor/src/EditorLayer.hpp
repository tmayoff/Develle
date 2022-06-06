#ifndef EDITORLAYER_H_
#define EDITORLAYER_H_

#include <Develle.hpp>
#include <Develle/Scene/Scene.hpp>
#include <Panels/FileBrowser.hpp>
#include <Panels/SceneHierarchyPanel.hpp>

namespace Develle {

class EditorLayer : public Layer {
 public:
  void OnAttach() override;
  void OnDetach() override;

  void OnUpdate(Timestep deltaTime) override;
  void OnImGuiRender() override;
  void OnEvent(Event &e) override;

 private:
  void OpenProject();
  void OpenScene();
  void OpenScene(const std::filesystem::path &path);
  void SaveSceneAs();
  auto OnKeyPressed(KeyPressedEvent &e) -> bool;
  auto OnMouseButtonPressed(MouseButtonPressedEvent &e) -> bool;

  std::filesystem::path openProject = "/home/tyler/Documents/Develle";

  Ref<Scene> activeScene;

  // Entity
  Entity camera;
  Entity hoveredEntity;

  EditorCamera editorCamera;

  // Guizmos
  int guizmoOperation = -1;

  SceneHierarchyPanel sceneHierarchyPanel;
  FileBrowser fileBrowser;
  Ref<Framebuffer> framebuffer;

  bool viewportFocused = false, viewportHovered = false;
  glm::vec2 viewportSize = {0.0F, 0.0F};
  std::array<glm::vec2, 2> viewportBounds{};
};

}  // namespace Develle
#endif  // EDITORLAYER_H_
