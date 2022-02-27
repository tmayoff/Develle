#pragma once
#ifndef RENDERER3D_HPP_
#define RENDERER3D_HPP_

#include "Camera.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

namespace Develle {

class Renderer3D {
 public:
  static void Init();

  static void BeginScene(const Camera& camera, const std::vector<Light>& lights);
  static void EndScene();

  static void DrawMesh(Mesh mesh, const glm::vec3& position, const glm::vec3& rotation,
                       const glm::vec3& scale, const glm::vec4& color);

  static void DrawMesh(Mesh mesh, const glm::vec3& position, const glm::vec3& rotation,
                       const glm::vec3& scale, const Ref<Texture2D>& texture,
                       const glm::vec4& color);
  static void DrawMesh(Mesh mesh, const glm::mat4& transform, const glm::vec4& color);
  static void DrawMesh(Mesh mesh, const glm::mat4& transform, const Ref<Texture2D>& texture,
                       const glm::vec4& tintColor);
};

}  // namespace Develle
#endif  // RENDERER3D_HPP_
