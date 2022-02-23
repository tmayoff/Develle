#include "Sandbox3D.hpp"

#include <Develle.hpp>
#include <glm/ext/matrix_transform.hpp>

Sandbox3D::Sandbox3D() {}

void Sandbox3D::OnAttach() {
  camera = Develle::EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);  // NOLINT
  mesh = Develle::Mesh::CreateQuadMesh();

  checkerboard = Develle::Texture2D::Create(
      "/home/tyler/src/Develle/Sandbox/assets/textures/Checkerboard.png");
}

void Sandbox3D::OnDetach() {}

void Sandbox3D::OnUpdate(Develle::Timestep delta) {
  camera.OnUpdate(delta);

  Develle::Renderer3D::BeginScene(camera.GetCamera());

  Develle::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});  // NOLINT
  Develle::RenderCommand::Clear();

  Develle::Renderer3D::DrawMesh(mesh, {0.0f, 0.0f, 0.0f}, {-90.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
                                checkerboard, {1.0f, 1.0f, 1.0f, 1.0f});

  Develle::Renderer3D::EndScene();
}

void Sandbox3D::OnImGuiRender() {}

void Sandbox3D::OnEvent(Develle::Event& e) { camera.OnEvent(e); }
