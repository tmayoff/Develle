#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Develle {

struct TagComponent {
  std::string Tag;

  TagComponent() = default;
  TagComponent(const TagComponent &) = default;
  TagComponent(const std::string &tag) : Tag(tag) {}
};

struct TransformComponent {
  glm::vec3 Position = {0.0f, 0.0f, 0.0f};
  glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
  glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

  TransformComponent() = default;
  TransformComponent(const TransformComponent &) = default;
  TransformComponent(const glm::vec3 &position) : Position(position) {}

  glm::mat4 GetTransform() const {
    glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

    return glm::translate(glm::mat4(1.0f), Position) * rotation *
           glm::scale(glm::mat4(1.0f), Scale);
  }
};

} // namespace Develle
#endif // COMPONENTS_H_
