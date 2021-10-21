#include <Panels/SceneHierarchyPanel.h>

#include <imgui.h>

#include <Develle/Scene/Components.h>

namespace Develle {

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &context) {
  SetContext(context);
}

void SceneHierarchyPanel::SetContext(const Ref<Scene> &context) {
  this->context = context;
  selectionContext = {};
}

void SceneHierarchyPanel::OnImGuiRender() {
  ImGui::Begin("Scene Hierarchy");
  context->registry.each([&](auto entityID) {
    Entity entity{entityID, context.get()};
    DrawEntityNode(entity);
  });

  if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
    selectionContext = {};

  if (ImGui::BeginPopupContextWindow(0, 1, false)) {
    if (ImGui::MenuItem("Create Empty Entity"))
      context->CreateEntity("Empty Entity");

    ImGui::EndPopup();
  }

  ImGui::End();

  ImGui::Begin("Properties");
  if (selectionContext)
    DrawComponents(selectionContext);

  ImGui::End();
}

void SceneHierarchyPanel::SetSelectedEntity(Entity entity) {
  selectionContext = entity;
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
  auto &tag = entity.GetComponent<TagComponent>().Tag;
}

void SceneHierarchyPanel::DrawComponents(Entity entity) {}

template <typename T, typename UIFunction>
static void DrawComponent(const std::string &name, Entity entity,
                          UIFunction uiFunction) {}
} // namespace Develle
