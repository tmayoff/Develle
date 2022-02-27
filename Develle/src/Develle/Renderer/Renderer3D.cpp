#include "Renderer3D.hpp"

#include <Develle/Core/Core.hpp>
#include <utility>

#include "RenderCommand.hpp"
#include "UniformBuffer.hpp"

namespace Develle {

struct Render3DData {
  Ref<Texture2D> WhiteTexture;

  struct CameraData {
    glm::mat4 ViewProjection;
  };

  struct LightData {
    float intensity = 0.3F;
    glm::vec3 position;
    glm::vec3 color;
  };

  Ref<Shader> shader;

  LightData LightBuffer{};
  CameraData CameraBuffer{};
  Ref<UniformBuffer> LightUniformBuffer;
  Ref<UniformBuffer> CameraUniformBuffer;
};

static Render3DData data3D;  // NOLINT

void Renderer3D::Init() {
  data3D.WhiteTexture = Texture2D::Create(1, 1);
  static uint32_t whiteTexture = 0xFFFFFFFF;  // NOLINT
  data3D.WhiteTexture->SetData(&whiteTexture, 4);

  data3D.CameraUniformBuffer = UniformBuffer::Create(sizeof(Render3DData::CameraData), 0);
  data3D.LightUniformBuffer = UniformBuffer::Create(sizeof(Render3DData::LightData), 1);

  data3D.shader = Shader::Create("shaders/Texture.glsl");
  data3D.shader->Bind();
}

void Renderer3D::BeginScene(const Camera& camera, const std::vector<Light>& lights) {
  data3D.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
  data3D.CameraUniformBuffer->SetData(&data3D.CameraBuffer, sizeof(Render3DData::CameraData));

  for (auto l : lights) {
    data3D.LightBuffer.intensity = 0.7F;
    data3D.LightBuffer.position = l.GetPosition();
    data3D.LightBuffer.color = l.GetColor();
    data3D.LightUniformBuffer->SetData(&data3D.LightBuffer, sizeof(Render3DData::LightData));
  }
}

void Renderer3D::EndScene() {}

void Renderer3D::DrawMesh(Mesh mesh, const glm::vec3& position, const glm::vec3& rotation,
                          const glm::vec3& scale, const glm::vec4& color) {
  glm::mat4 transform = glm::translate(glm::mat4(1.0F), position) *
                        glm::toMat4(glm::quat(glm::radians(rotation))) *
                        glm::scale(glm::mat4(1.0F), scale);
  DrawMesh(std::move(mesh), transform, color);
}

void Renderer3D::DrawMesh(Mesh mesh, const glm::vec3& position, const glm::vec3& rotation,
                          const glm::vec3& scale, const Ref<Texture2D>& texture,
                          const glm::vec4& color) {
  glm::mat4 transform = glm::translate(glm::mat4(1.0F), position) *
                        glm::toMat4(glm::quat(glm::radians(rotation))) *
                        glm::scale(glm::mat4(1.0F), scale);
  DrawMesh(std::move(mesh), transform, texture, color);
}

void Renderer3D::DrawMesh(Mesh mesh, const glm::mat4& transform, const glm::vec4& color) {
  std::vector<MeshVertex> vertices = mesh.GetVertices();
  for (auto& vertice : vertices) {
    vertice.Position = transform * vertice.Position;
    vertice.Color = color;
  }
  mesh.GetVertexBuffer()->SetData(vertices.data(), vertices.size() * sizeof(MeshVertex));

  data3D.WhiteTexture->Bind();
  mesh.GetVertexArray()->Bind();
  RenderCommand::DrawIndexed(mesh.GetVertexArray(), mesh.GetIndexCount());
}

void Renderer3D::DrawMesh(Mesh mesh, const glm::mat4& transform, const Ref<Texture2D>& texture,
                          const glm::vec4& tintColor) {
  std::vector<MeshVertex> vertices(mesh.GetVertices().size());
  for (size_t i = 0; i < vertices.size(); i++) {
    vertices[i].Position = transform * mesh.GetVertices()[i].Position;
    vertices[i].TexCoord = mesh.GetVertices()[i].TexCoord;
    vertices[i].Color = tintColor;
  }
  mesh.GetVertexBuffer()->SetData(vertices.data(), vertices.size() * sizeof(MeshVertex));

  texture->Bind();
  mesh.GetVertexArray()->Bind();
  RenderCommand::DrawIndexed(mesh.GetVertexArray(), mesh.GetIndexCount());
}

}  // namespace Develle
