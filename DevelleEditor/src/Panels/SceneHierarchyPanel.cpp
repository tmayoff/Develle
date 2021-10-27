#include <Panels/SceneHierarchyPanel.hpp>

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

  // Right-Click
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

  ImGuiTreeNodeFlags flags =
      ((selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
      ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

  bool opened =
      ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, flags, tag.c_str());

  if (ImGui::IsItemClicked())
    selectionContext = entity;

  bool entityDeleted = false;
  if (ImGui::BeginPopupContextItem()) {
    if (ImGui::MenuItem("Delete Entity"))
      entityDeleted = true;

    ImGui::EndPopup();
  }

  if (opened) {
    // TODO(tyler) Recursively DrawEntityNodes for children nodes
  }

  if (entityDeleted) {
    context->DestroyEntity(entity);
    if (selectionContext == entity)
      selectionContext = {};
  }
}

void SceneHierarchyPanel::DrawComponents(Entity entity) {
  if (entity.HasComponent<TagComponent>()) {
    auto &tag = entity.GetComponent<TagComponent>().Tag;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    std::strncpy(buffer, tag.c_str(), sizeof(buffer));
    if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
      tag = std::string(buffer);
  }

  ImGui::SameLine();
  ImGui::PushItemWidth(-1);

  if (ImGui::Button("Add Component"))
    ImGui::OpenPopup("AddComponent");

  if (ImGui::BeginPopup("AddComponent")) {

    ImGui::EndPopup();
  }

  ImGui::PopItemWidth();
}

template <typename T, typename UIFunction>
static void DrawComponent(const std::string &name, Entity entity,
                          UIFunction uiFunction) {}
} // namespace Develle
