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
};

class Mesh {
 public:
  static Mesh CreateQuadMesh();
  static Mesh CreateCubeMesh();

  const Ref<Shader>& GetShader() { return shader; }
  const Ref<VertexBuffer>& GetVertexBuffer() { return vertexBuffer; }
  const Ref<VertexArray>& GetVertexArray() { return vertexArray; }
  void SetTexCoords(const std::vector<glm::vec2>& coords);
  const std::vector<glm::vec2>& GetTexCoords() { return texCoords; }
  const std::vector<MeshVertex>& GetVertices() { return vertices; }
  uint32_t GetIndexCount() { return indexCount; }

 private:
  std::vector<MeshVertex> vertices;
  std::vector<glm::vec2> texCoords;
  Ref<Shader> shader;
  Ref<VertexArray> vertexArray;
  Ref<VertexBuffer> vertexBuffer;
  uint32_t indexCount;
};

}  // namespace Develle
#endif  // MESH_HPP_
