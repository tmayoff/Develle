#pragma once
#ifndef MESH_HPP_
#define MESH_HPP_

#include <Develle/Core/Core.hpp>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "VertexArray.hpp"

namespace Develle {

struct MeshVertex {
  glm::vec4 Position;
  glm::vec4 Color;
  glm::vec2 TexCoord;
  glm::vec3 Normal;
};

class Mesh {
 public:
  static auto CreateQuadMesh() -> Mesh;
  static auto CreateCubeMesh() -> Mesh;

  Mesh() = default;
  Mesh(std::vector<MeshVertex> meshVertices, std::vector<uint32_t> indices);

  auto GetVertexBuffer() -> const Ref<VertexBuffer>& { return vertexBuffer; }

  auto GetVertexArray() -> const Ref<VertexArray>& { return vertexArray; }

  // void SetTexCoords(const std::vector<glm::vec2>& coords);
  auto GetTexCoords() -> const std::vector<glm::vec2>& { return texCoords; }

  auto GetVertices() -> const std::vector<MeshVertex>& { return vertices; }

  [[nodiscard]] auto GetIndexCount() const -> uint32_t { return indexCount; }

 private:
  std::vector<MeshVertex> vertices;
  std::vector<glm::vec2> texCoords;
  Ref<VertexArray> vertexArray;
  Ref<VertexBuffer> vertexBuffer;
  uint32_t indexCount;
};

}  // namespace Develle
#endif  // MESH_HPP_
