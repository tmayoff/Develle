#ifndef SCENEHIERACHYPANEL_H_
#define SCENEHIERACHYPANEL_H_

#include <Develle/Core/Core.h>
#include <Develle/Scene/Entity.h>
#include <Develle/Scene/Scene.h>

namespace Develle {

class SceneHierarchyPanel {
 public:
  SceneHierarchyPanel() = default;
  SceneHierarchyPanel(const Ref<Scene> &scene);

  void SetContext(const Ref<Scene> &scene);

  void OnImGuiRender();

  Entity GetSelectedEntity() const { return selectionContext; }
  void SetSelectedEntity(Entity entity);

 private:
  void DrawEntityNode(Entity entity);
  void DrawComponents(Entity entity);

  Ref<Scene> context;
  Entity selectionContext;
};

}  // namespace Develle
#endif  // SCENEHIERACHYPANEL_H_
