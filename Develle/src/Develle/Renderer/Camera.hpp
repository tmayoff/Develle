#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

  const glm::mat4 &GetProjection() const { return projection; }
  void SetProjection(const glm::mat4 &mat) { projection = mat; }

  glm::mat4 GetViewProjection() const {
    DV_PROFILE_FUNCTION();  // NOLINT
    return projection * glm::inverse(glm::translate(glm::mat4(1.0), position));
  }

 private:
  glm::mat4 projection = glm::mat4(1.0f);  // NOLINT
  glm::vec3 position = glm::vec3(0.0f);
};

}  // namespace Develle
#endif  // CAMERA_HPP_
