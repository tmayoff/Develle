#include <imgui.h>
#include <imgui_internal.h>

#include <Develle/Scene/Components.hpp>
#include <Panels/SceneHierarchyPanel.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Develle {

std::filesystem::path assetPath;

template <typename T, typename UIFunction>
static void DrawComponent(const std::string &name, Entity entity, UIFunction uiFunction) {
  const ImGuiTreeNodeFlags treeNodeFlags =
      ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
      ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
      ImGuiTreeNodeFlags_FramePadding;

  if (entity.HasComponent<T>()) {
    auto &component = entity.GetComponent<T>();
    ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;  // NOLINT
    ImGui::Separator();
    bool open = ImGui::TreeNodeEx((void *)typeid(T).hash_code(), treeNodeFlags, "%s",  // NOLINT
                                  name.c_str());                                       // NOLINT
    ImGui::PopStyleVar();
    ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);  // NOLINT
    if (ImGui::Button("+", ImVec2{lineHeight, lineHeight})) ImGui::OpenPopup("ComponentSettings");

    bool removeComponent = false;
    if (ImGui::BeginPopup("ComponentSettings")) {
      if (ImGui::MenuItem("Remove component")) removeComponent = true;
      ImGui::EndPopup();
    }

    if (open) {
      uiFunction(component);
      ImGui::TreePop();
    }

    if (removeComponent) entity.RemoveComponent<T>();
  }
}

static void DrawVec3Control(const std::string &label, glm::vec3 &values, float resetValue = 0.0f,
                            float columnWidth = 100.0f) {
  ImGuiIO &io = ImGui::GetIO();
  auto boldFont = io.Fonts->Fonts[0];

  ImGui::PushID(label.c_str());

  ImGui::Columns(2);
  ImGui::SetColumnWidth(0, columnWidth);
  ImGui::Text("%s", label.c_str());  // NOLINT
  ImGui::NextColumn();

  ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

  float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;  // NOLINT
  ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};                              // NOLINT

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});        // NOLINT
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});  // NOLINT
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});  // NOLINT
  ImGui::PushFont(boldFont);
  if (ImGui::Button("X", buttonSize)) values.x = resetValue;
  ImGui::PopFont();
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");  // NOLINT
  ImGui::PopItemWidth();
  ImGui::SameLine();

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});         // NOLINT
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});  // NOLINT
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});   // NOLINT
  ImGui::PushFont(boldFont);
  if (ImGui::Button("Y", buttonSize)) values.y = resetValue;
  ImGui::PopFont();
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");  // NOLINT
  ImGui::PopItemWidth();
  ImGui::SameLine();

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});         // NOLINT
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});  // NOLINT
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});   // NOLINT
  ImGui::PushFont(boldFont);
  if (ImGui::Button("Z", buttonSize)) values.z = resetValue;  // NOLINT
  ImGui::PopFont();
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");  // NOLINT
  ImGui::PopItemWidth();
  ImGui::SameLine();

  ImGui::PopStyleVar();

  ImGui::Columns(1);

  ImGui::PopID();
}

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &context) { SetContext(context); }

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

  if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) selectionContext = {};

  // Right-Click
  if (ImGui::BeginPopupContextWindow(0, 1, false)) {
    if (ImGui::MenuItem("Create Empty Entity")) context->CreateEntity("Empty Entity");

    ImGui::EndPopup();
  }

  ImGui::End();

  ImGui::Begin("Properties");
  if (selectionContext) DrawComponents(selectionContext);

  ImGui::End();
}

void SceneHierarchyPanel::SetSelectedEntity(Entity entity) { selectionContext = entity; }

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
  auto &tag = entity.GetComponent<TagComponent>().Tag;

  ImGuiTreeNodeFlags flags = ((selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
                             ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

  bool opened =
      ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());  // NOLINT

  if (ImGui::IsItemClicked()) selectionContext = entity;

  bool entityDeleted = false;
  if (ImGui::BeginPopupContextItem()) {
    if (ImGui::MenuItem("Delete Entity")) entityDeleted = true;

    ImGui::EndPopup();
  }

  if (opened) {
    // TODO(tyler) Recursively DrawEntityNodes for children nodes
    ImGui::TreePop();
  }

  if (entityDeleted) {
    context->DestroyEntity(entity);
    if (selectionContext == entity) selectionContext = {};
  }
}

void SceneHierarchyPanel::DrawComponents(Entity entity) {
  if (entity.HasComponent<TagComponent>()) {
    auto &tag = entity.GetComponent<TagComponent>().Tag;
    ImGui::InputText("##Tag", tag.data(), 256);  // NOLINT
  }

  ImGui::SameLine();
  ImGui::PushItemWidth(-1);

  if (ImGui::Button("Add Component")) ImGui::OpenPopup("AddComponent");

  if (ImGui::BeginPopup("AddComponent")) {
    if (ImGui::MenuItem("Camera")) {
      if (!selectionContext.HasComponent<CameraComponent>())
        selectionContext.AddComponent<CameraComponent>();
      else
        DV_CORE_WARN("Entity already has Scene camera");

      ImGui::CloseCurrentPopup();
    }

    if (ImGui::MenuItem("Sprite Renderer")) {
      if (!selectionContext.HasComponent<SpriteRendererComponent>())
        selectionContext.AddComponent<SpriteRendererComponent>();
      else
        DV_CORE_WARN("Entity already has Sprite renderer");

      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  ImGui::PopItemWidth();

  DrawComponent<TransformComponent>("Transform", entity, [](auto &component) {
    DrawVec3Control("Position", component.Position);
    glm::vec3 rotation = glm::degrees(component.Rotation);
    DrawVec3Control("Rotation", rotation);
    component.Rotation = glm::radians(rotation);
    DrawVec3Control("Scale", component.Scale, 1.0f);
  });

  DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto &component) {
    ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

    ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
    if (ImGui::BeginDragDropTarget()) {
      if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
        const char *path = (const char *)payload->Data;
        std::filesystem::path texturePath = std::filesystem::path(assetPath) / path;
        component.Texture = Texture2D::Create(texturePath.string());
      }
    }
    ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
  });
}

}  // namespace Develle
