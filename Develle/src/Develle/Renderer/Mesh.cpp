#include "Mesh.hpp"

namespace Develle {

Mesh Mesh::CreateQuadMesh() {
  Mesh mesh;

  mesh.vertexArray = VertexArray::Create();

  mesh.vertexBuffer = VertexBuffer::Create(4 * sizeof(MeshVertex));
  mesh.vertexBuffer->SetLayout({
      {ShaderDataType::Float4, "a_Position"},
      {ShaderDataType::Float4, "a_Color"},
      {ShaderDataType::Float2, "a_TexCoord"},
  });

  // Vertices
  mesh.vertices = std::vector<MeshVertex>(4);
  mesh.vertices[0] = {
      {-0.5f, -0.5f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}};                 // NOLINT
  mesh.vertices[1] = {{0.5f, -0.5f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}};  // NOLINT
  mesh.vertices[2] = {{0.5f, 0.5f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}};   // NOLINT
  mesh.vertices[3] = {{-0.5f, 0.5f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};  // NOLINT
  mesh.vertexBuffer->SetData(mesh.vertices.data(), mesh.vertices.size() * sizeof(MeshVertex));
  mesh.vertexArray->AddVertexBuffer(mesh.vertexBuffer);

  std::array<uint32_t, 6> indices{};
  mesh.indexCount = indices.size();
  indices[0] = 0;
  indices[1] = 1;
  indices[2] = 2;
  indices[3] = 2;
  indices[4] = 3;
  indices[5] = 0;
  auto ib = IndexBuffer::Create(indices.data(), indices.size());

  mesh.vertexArray->SetIndexBuffer(ib);

  mesh.shader = Shader::Create("shaders/Texture.glsl");

  return mesh;
}

}  // namespace Develle
