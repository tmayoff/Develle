#include "Sandbox3D.hpp"

#include <Develle.hpp>
#include <glm/ext/matrix_transform.hpp>

void Sandbox3D::OnAttach() {
  camera = Develle::EditorCamera(30.0F, 1.778F, 0.1F, 1000.0F);
  mesh = Develle::Mesh::CreateQuadMesh();

  light = Develle::Light({0.0F, 10.0F, 0.0F}, {1.0F, 0.0F, 0.0F});

  cube = Develle::Mesh::CreateCubeMesh();

  checkerboard = Develle::Texture2D::Create(
      "/home/tyler/src/Develle/Sandbox/assets/textures/prototype_512x512_blue.png");
}

void Sandbox3D::OnDetach() {}

void Sandbox3D::OnUpdate(Develle::Timestep delta) {
  camera.OnUpdate(delta);

  Develle::Renderer3D::BeginScene(camera.GetCamera(), {light});

  Develle::RenderCommand::SetClearColor({0.1F, 0.1F, 0.1F, 1.0F});
  Develle::RenderCommand::Clear();

  // Floor
  Develle::Renderer3D::DrawMesh(mesh, {0.0F, -1.0F, 0.0F}, {-90.0F, 0.0F, 0.0F},
                                {15.0F, 15.0F, 1.0F}, checkerboard, {1.0F, 1.0F, 1.0F, 1.0F});

  Develle::Renderer3D::DrawMesh(cube, {0.0F, 0.0F, 0.0F}, {00.0F, 0.0F, 0.0F}, {1.0F, 1.0F, 1.0F},
                                {1.0F, 1.0F, 1.0F, 1.0F});

  Develle::Renderer3D::EndScene();
}

void Sandbox3D::OnImGuiRender() {}

void Sandbox3D::OnEvent(Develle::Event& e) { camera.OnEvent(e); }
