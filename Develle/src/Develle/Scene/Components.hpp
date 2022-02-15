#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include <Develle/Core/Core.hpp>
#include <Develle/Renderer/Texture.hpp>
#include <Develle/Scene/SceneCamera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Develle {

struct TagComponent {
  TagComponent(const std::string &tag) : Tag(tag) {}

  std::string Tag;
};

struct TransformComponent {
  TransformComponent(const glm::vec3 &position = glm::vec3(0.0f)) : Position(position) {}

  glm::vec3 Position = {0.0f, 0.0f, 0.0f};
  glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
  glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

  glm::mat4 GetTransform() const {
    glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

    return glm::translate(glm::mat4(1.0f), Position) * rotation *
           glm::scale(glm::mat4(1.0f), Scale);
  }
};

struct SpriteRendererComponent {
  SpriteRendererComponent(const glm::vec4 &color = glm::vec4(1.0f)) : Color(color) {}

  glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
  Ref<Texture2D> Texture;
  float TilingFactor = 1.0f;
};

struct CameraComponent {
  SceneCamera Camera;
  bool Primary = true;
  bool FixedAspectRatio = false;
};

}  // namespace Develle

#endif  // COMPONENTS_HPP_
