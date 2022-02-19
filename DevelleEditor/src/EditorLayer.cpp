#include <imgui.h>
//
#include <ImGuizmo.h>

#include <Develle/Scene/SceneSerializer.hpp>
#include <Develle/Utils/PlatformUtils.hpp>
#include <EditorLayer.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Develle {

EditorLayer::EditorLayer() : Layer("Editor Layer") {}

void EditorLayer::OnAttach() {
  DV_PROFILE_FUNCTION();  // NOLINT

  FramebufferSpecification fbSpec;
  fbSpec.Attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER,
                        FramebufferTextureFormat::Depth};
  fbSpec.Width = 1280;  // NOLINT
  fbSpec.Height = 720;  // NOLINT
  framebuffer = Framebuffer::Create(fbSpec);

  activeScene = CreateRef<Scene>();

  editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);  // NOLINT

  OpenScene(std::string(ASSETS_ROOT) + "/example.develle");

  sceneHierarchyPanel.SetContext(activeScene);
}

void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate(Timestep deltaTime) {
  DV_PROFILE_FUNCTION();  // NOLINT

  // Resize framebuffer
  FramebufferSpecification spec = framebuffer->GetSpecification();
  if (viewportSize.x > 0.0f && viewportSize.y > 0.0f &&                                 // NOLINT
      (spec.Width != viewportSize.x || spec.Height != viewportSize.y)) {                // NOLINT
    framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);            // NOLINT
    editorCamera.SetViewportSize(viewportSize.x, viewportSize.y);                       // NOLINT
    activeScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);  // NOLINT
  }

  // Render
  Renderer2D::ResetStats();

  // Clear
  framebuffer->Bind();

  RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});  // NOLINT
  RenderCommand::Clear();

  framebuffer->ClearAttachment(1, -1);

  if (!ImGuizmo::IsUsing()) editorCamera.OnUpdate(deltaTime);
  activeScene->OnUpdateEditor(deltaTime, editorCamera);

  // Mouse picking
  auto [mx, my] = ImGui::GetMousePos();
  mx -= viewportBounds[0].x;  // NOLINT
  my -= viewportBounds[0].y;  // NOLINT
  glm::vec2 viewportSize = viewportBounds[1] - viewportBounds[0];
  my = viewportSize.y - my;  // NOLINT
  int mouseX = (int)mx;
  int mouseY = (int)my;
  if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x &&  // NOLINT
      mouseY < (int)viewportSize.y) {                                // NOLINT
    int pixelData = framebuffer->ReadPixel(1, mouseX, mouseY);
    if (pixelData != -1)
      hoveredEntity = Entity((entt::entity)pixelData, activeScene.get());
    else if (!editorCamera.IsUsing())
      hoveredEntity = Entity();
  }

  framebuffer->Unbind();
}

void EditorLayer::OnImGuiRender() {
  DV_PROFILE_FUNCTION();  // NOLINT

  bool optFullscreen = true;
  static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if (optFullscreen) {
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  }

  if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
    windowFlags |= ImGuiWindowFlags_NoBackground;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  static bool dockspaceOpen = true;
  ImGui::Begin("Develle Editor", &dockspaceOpen, windowFlags);
  ImGui::PopStyleVar();

  if (optFullscreen) ImGui::PopStyleVar(2);

  // Dockspace
  ImGuiIO &io = ImGui::GetIO();
  ImGuiStyle &style = ImGui::GetStyle();
  float minWinSizeX = style.WindowMinSize.x;
  style.WindowMinSize.x = 370.0f;  // NOLINT
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGuiID dockspaceID = ImGui::GetID("Develle Editor");
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
  }

  style.WindowMinSize.x = minWinSizeX;

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "Ctrl+N")) {
      }

      if (ImGui::MenuItem("Open...", "Ctrl+O")) OpenScene();

      if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) SaveSceneAs();

      if (ImGui::MenuItem("Exit")) Application::Get().Close();
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  sceneHierarchyPanel.OnImGuiRender();
  fileBrowser.OnImGuiRender();

  {
    ImGui::Begin("Stats");
    std::string name = "None";
    if (hoveredEntity) name = hoveredEntity.GetComponent<TagComponent>().Tag;
    ImGui::Text("Hovered Entity: %s", name.c_str());  // NOLINT

    auto stats = Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");                          // NOLINT
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);            // NOLINT
    ImGui::Text("Quads: %d", stats.QuadCount);                 // NOLINT
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());  // NOLINT
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());    // NOLINT

    ImGui::End();
  }

  {
    ImGui::Begin("ImGuizmo");

    ImGui::Text("Using: %s", ImGuizmo::IsUsing() ? "true" : "false");  // NOLINT
    switch ((ImGuizmo::OPERATION)guizmoOperation) {
      case ImGuizmo::OPERATION::TRANSLATE:
        ImGui::Text("Operation: %s", "translate");  // NOLINT
        break;
      case ImGuizmo::OPERATION::SCALE:
        ImGui::Text("Operation: %s", "scale");  // NOLINT
        break;
      case ImGuizmo::OPERATION::ROTATE:
        ImGui::Text("Operation: %s", "rotate");  // NOLINT
        break;
      default:
        ImGui::Text("Operation: %s", "none");  // NOLINT
        break;
    }

    ImGui::End();
  }

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.0f, 0.0f});
  ImGui::Begin("Viewport");
  auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
  auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
  auto viewportOffset = ImGui::GetWindowPos();
  viewportBounds[0] = {viewportMinRegion.x + viewportOffset.x,
                       viewportMinRegion.y + viewportOffset.y};
  viewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x,
                       viewportMaxRegion.y + viewportOffset.y};

  viewportFocused = ImGui::IsWindowFocused();
  viewportHovered = ImGui::IsWindowHovered();
  Application::Get().GetImGuiLayer()->BlockEvents(!viewportFocused && !viewportHovered);

  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
  viewportSize = {viewportPanelSize.x, viewportPanelSize.y};

  uint64_t textureID = framebuffer->GetColorAttachmentRendererID();
  ImGui::Image(reinterpret_cast<void *>(textureID),                                  // NOLINT
               ImVec2{viewportSize.x, viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});  // NOLINT
  {
    // Guizmos
    const glm::mat4 &cameraProjection = editorCamera.GetCamera().GetProjectionMatrix();
    glm::mat4 cameraView = editorCamera.GetCamera().GetViewMatrix();

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y,    // NOLINT
                      viewportBounds[1].x - viewportBounds[0].x,   // NOLINT
                      viewportBounds[1].y - viewportBounds[0].y);  // NOLINT

    // Cube
    ImGuizmo::ViewManipulate(glm::value_ptr(cameraView), 8,               // NOLINT
                             {viewportBounds[0].x, viewportBounds[0].y},  // NOLINT
                             ImVec2{100, 100},                            // NOLINT
                             0x10101011);                                 // NOLINT

    // Transform controls
    Entity selectedEntity = sceneHierarchyPanel.GetSelectedEntity();
    if (selectedEntity && guizmoOperation != -1) {
      auto &tc = selectedEntity.GetComponent<TransformComponent>();
      glm::mat4 transform = tc.GetTransform();

      bool snap = Input::IsKeyPressed(Key::LCTRL);
      float snapValue = 0.5f;                                                 // NOLINT
      if (guizmoOperation == ImGuizmo::OPERATION::ROTATE) snapValue = 45.0f;  // NOLINT
      std::array<float, 3> snapValues = {snapValue, snapValue, snapValue};

      ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                           (ImGuizmo::OPERATION)guizmoOperation, ImGuizmo::LOCAL,
                           glm::value_ptr(transform), nullptr, snap ? snapValues.data() : nullptr);

      if (ImGuizmo::IsUsing()) {
        glm::vec3 position, rotation, scale;
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(position),
                                              glm::value_ptr(rotation), glm::value_ptr(scale));
        glm::vec3 deltaRot = rotation - tc.Rotation;
        tc.Position = position;
        tc.Rotation += deltaRot;
        tc.Scale = scale;
      }
    }
  }

  ImGui::End();
  ImGui::PopStyleVar();

  ImGui::End();
}

void EditorLayer::OnEvent(Event &e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<KeyPressedEvent>(DV_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
  dispatcher.Dispatch<MouseButtonPressedEvent>(DV_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));

  editorCamera.OnEvent(e);
}

void EditorLayer::OpenScene() {
  auto file = FileDialogs::OpenFile({"Develle Scene (*.develle)", "*.develle"});
  if (!file.empty()) OpenScene(file);
}

void EditorLayer::OpenScene(const std::filesystem::path &path) {
  if (path.extension().string() != ".develle") {
    DV_CORE_WARN("Could not load {0} - not a scene file", path.filename().string());
    return;
  }

  Ref<Scene> newScene = CreateRef<Scene>();
  SceneSerializer serializer(newScene);
  if (serializer.Deserialize(path.string())) {
    activeScene = newScene;
    activeScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);  // NOLINT
    sceneHierarchyPanel.SetContext(activeScene);
  }
}

void EditorLayer::SaveSceneAs() {
  std::filesystem::path filepath =
      FileDialogs::SaveFile({"Develle Scene (*.develle)", "*.develle"});
  if (!filepath.empty()) {
    SceneSerializer serializer(activeScene);
    serializer.Serialize(filepath.string());
  }
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent &e) {
  if (e.GetRepeatCount() > 0) return false;

  bool ctrl = Input::IsKeyPressed(Key::LCTRL) || Input::IsKeyPressed(Key::RCTRL);
  bool shift = Input::IsKeyPressed(Key::LSHIFT) || Input::IsKeyPressed(Key::RSHIFT);

  switch (e.GetKeyCode()) {
    case Key::N:
      // if (Control) NewScene();
      break;
    case Key::O:
      if (ctrl) OpenScene();
      break;
    case Key::S:
      if (ctrl && shift) SaveSceneAs();
      break;

    // Guizmo
    case Key::Q:
      if (!ImGuizmo::IsUsing()) guizmoOperation = -1;
      break;
    case Key::W:
      if (!ImGuizmo::IsUsing()) guizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
      break;
    case Key::E:
      if (!ImGuizmo::IsUsing()) guizmoOperation = ImGuizmo::OPERATION::SCALE;
      break;
    case Key::R:
      if (!ImGuizmo::IsUsing()) guizmoOperation = ImGuizmo::OPERATION::ROTATE;
      break;
  }

  return false;
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent &) {
  if (viewportHovered) sceneHierarchyPanel.SetSelectedEntity(hoveredEntity);
  return false;
}

}  // namespace Develle
