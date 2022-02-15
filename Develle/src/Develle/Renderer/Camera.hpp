#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <Develle/Debug/Instrumentor.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Develle {

class Camera {
 public:
  Camera() = default;
  Camera(Camera &&cam) : projection(cam.projection) {}
  Camera(const Camera &cam) : projection(cam.projection) {}
  Camera(const glm::mat4 &projection) : projection(projection) {}
  Camera &operator=(const Camera &) = default;
  Camera &operator=(Camera &&) = default;

  virtual ~Camera() = default;

  void SetPosition(const glm::vec3 &pos) { position = pos; }
  glm::vec3 GetPosition() { return position; }

  void SetRotation(const glm::quat &rot) { rotation = rot; }
  glm::quat GetRotation() { return rotation; }

  glm::mat4 GetViewProjectionMatrix() const { return projection * GetViewMatrix(); }

  const glm::mat4 &GetProjectionMatrix() const { return projection; }
  void SetProjection(const glm::mat4 &mat) { projection = mat; }

  glm::mat4 GetViewMatrix() const {
    DV_PROFILE_FUNCTION();  // NOLINT
    return glm::inverse(glm::translate(glm::mat4(1.0), position) * glm::toMat4(rotation));
  }

 private:
  glm::mat4 projection = glm::mat4(1.0f);  // NOLINT
  glm::vec3 position = glm::vec3(0.0f);
  glm::quat rotation{};
};

}  // namespace Develle
#endif  // CAMERA_HPP_
