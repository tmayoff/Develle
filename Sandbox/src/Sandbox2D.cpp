#include <Sandbox2D.hpp>

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), cameraController(1280.0f / 720.0f),
      squareColor({0.2f, 0.3f, 0.8f, 1.0f}) {}

void Sandbox2D::OnAttach() { DV_PROFILE_FUNCTION(); }

void Sandbox2D::OnDetach() { DV_PROFILE_FUNCTION(); }

void Sandbox2D::OnUpdate(Develle::Timestep delta) {
  DV_PROFILE_FUNCTION();

  cameraController.OnUpdate(delta);

  Develle::Renderer2D::ResetStats();
  {
    DV_PROFILE_SCOPE("Renderer Prep");
    Develle::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    Develle::RenderCommand::Clear();
  }

  {
    static float rotation = 0.0f;
    rotation += delta * 50.0f;

    DV_PROFILE_SCOPE("Renderer Draw");

    Develle::Renderer2D::BeginScene(cameraController.GetCamera());
    Develle::Renderer2D::DrawRotatedQuad({1.0f, 0.0f}, {0.8f, 0.8f}, -45.0f,
                                         {0.8f, 0.2f, 0.3f, 1.0f});
    Develle::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f},
                                  {0.8f, 0.2f, 0.3f, 1.0f});
    Develle::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.75f, 0.75f}, squareColor);
    Develle::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1}, {20.0f, 20.0f},
                                  checkerboardTexture, 10.0f);
    Develle::Renderer2D::DrawRotatedQuad({-2.0f, 0.0f, 0.0f}, {1.0f, 1.0f},
                                         rotation, checkerboardTexture, 20.0f);
    Develle::Renderer2D::EndScene();

    Develle::Renderer2D::BeginScene(cameraController.GetCamera());
    for (float y = -5.0f; y < 5.0f; y += 0.5f) {
      for (float x = -5.0f; x < 5.0f; x += 0.5f) {
        glm::vec4 color{(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f};
        Develle::Renderer2D::DrawQuad({x, y}, {0.45f, 0.45f}, color);
      }
    }
    Develle::Renderer2D::EndScene();
  }
}

void Sandbox2D::OnImGuiRender() {
  DV_PROFILE_FUNCTION();

  ImGui::Begin("Settings");

  auto stats = Develle::Renderer2D::GetStats();
  ImGui::Text("Renderer2D Stats:");
  ImGui::Text("Draw Calls: %d", stats.DrawCalls);
  ImGui::Text("Quads: %d", stats.QuadCount);
  ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
  ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

  ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));

  ImGui::End();
}

void Sandbox2D::OnEvent(Develle::Event &e) { cameraController.OnEvent(e); }
