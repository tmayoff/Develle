#include "Mesh.hpp"

#include <utility>

#include "ModelLoader.hpp"

namespace Develle {

auto Mesh::CreateQuadMesh() -> Mesh {
  Mesh mesh;

  mesh.vertexArray = VertexArray::Create();

  mesh.vertexBuffer = VertexBuffer::Create(4 * sizeof(MeshVertex));
  mesh.vertexBuffer->SetLayout({
      {ShaderDataType::Float4, "a_Position"},
      {ShaderDataType::Float4, "a_Color"},
      {ShaderDataType::Float2, "a_TexCoord"},
      {ShaderDataType::Float3, "a_Normal"},
  });

  // Vertices
  mesh.vertices = std::vector<MeshVertex>(4);
  mesh.vertices[0] = {
      {-0.5F, -0.5F, 0.0F, 1.0F}, {1.0F, 1.0F, 1.0F, 1.0F}, {0.0F, 0.0F}, {0.0F, 1.0F, 0.0F}};
  mesh.vertices[1] = {
      {0.5F, -0.5F, 0.0F, 1.0F}, {1.0F, 1.0F, 1.0F, 1.0F}, {1.0F, 0.0F}, {0.0F, 1.0F, 0.0F}};
  mesh.vertices[2] = {
      {0.5F, 0.5F, 0.0F, 1.0F}, {1.0F, 1.0F, 1.0F, 1.0F}, {1.0F, 1.0F}, {0.0F, 1.0F, 0.0F}};
  mesh.vertices[3] = {
      {-0.5F, 0.5F, 0.0F, 1.0F}, {1.0F, 1.0F, 1.0F, 1.0F}, {0.0F, 1.0F}, {0.0F, 1.0F, 0.0F}};
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

  return mesh;
}

auto Mesh::CreateCubeMesh() -> Mesh {
  return ModelLoader::LoadMesh("models/cube.obj");

  const std::array<glm::vec3, 8> cubeVertexPositions = {glm::vec3{-0.5F, -0.5F, -0.5F},
                                                        {0.5F, -0.5F, -0.5F},
                                                        {0.5F, 0.5F, -0.5F},
                                                        {-0.5F, 0.5F, -0.5F},
                                                        {-0.5F, -0.5F, 0.5F},
                                                        {0.5F, -0.5F, 0.5F},
                                                        {0.5F, 0.5F, 0.5F},
                                                        {-0.5F, 0.5F, 0.5F}};

  Mesh mesh;

  mesh.vertexArray = VertexArray::Create();

  mesh.vertexBuffer = VertexBuffer::Create(8 * sizeof(MeshVertex));
  mesh.vertexBuffer->SetLayout({
      {ShaderDataType::Float4, "a_Position"},
      {ShaderDataType::Float4, "a_Color"},
      {ShaderDataType::Float2, "a_TexCoord"},
      {ShaderDataType::Float3, "a_Normal"},
  });

  // Vertices
  mesh.vertices = std::vector<MeshVertex>(cubeVertexPositions.size());
  for (size_t i = 0; i < cubeVertexPositions.size(); i++) {
    mesh.vertices[i].Position = {cubeVertexPositions.at(i), 1.0F};
    mesh.vertices[i].Color = glm::vec4(1.0F);
    // TODO(tyler) Fix Texture coordinates for the cube
    mesh.vertices[i].TexCoord = {0.0F, 0.0F};
    mesh.vertices[i].Normal = {};
  }

  mesh.vertexBuffer->SetData(mesh.vertices.data(), mesh.vertices.size() * sizeof(MeshVertex));
  mesh.vertexArray->AddVertexBuffer(mesh.vertexBuffer);

  std::array<uint32_t, 36> indices = {0, 1, 3, 3, 1, 2, 1, 5, 2, 2, 5, 6, 5, 4, 6, 6, 4, 7,
                                      4, 0, 7, 7, 0, 3, 3, 2, 7, 7, 2, 6, 4, 5, 0, 0, 5, 1};
  mesh.indexCount = indices.size();
  auto ib = IndexBuffer::Create(indices.data(), indices.size());
  mesh.vertexArray->SetIndexBuffer(ib);

  return mesh;
}

Mesh::Mesh(std::vector<MeshVertex> meshVertices, std::vector<uint32_t> indices)
    : vertices(std::move(meshVertices)), indexCount(indices.size()) {
  vertexArray = VertexArray::Create();

  vertexBuffer = VertexBuffer::Create(vertices.size() * sizeof(MeshVertex));
  vertexBuffer->SetLayout({
      {ShaderDataType::Float4, "a_Position"},
      {ShaderDataType::Float4, "a_Color"},
      {ShaderDataType::Float2, "a_TexCoord"},
      {ShaderDataType::Float3, "a_Normal"},
  });

  // Vertices
  vertexBuffer->SetData(vertices.data(), vertices.size() * sizeof(MeshVertex));
  vertexArray->AddVertexBuffer(vertexBuffer);

  auto ib = IndexBuffer::Create(indices.data(), indices.size());

  vertexArray->SetIndexBuffer(ib);
}

}  // namespace Develle
