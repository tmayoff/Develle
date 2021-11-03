#ifndef SCENECAMERA_HPP_
#define SCENECAMERA_HPP_

#include <Develle/Renderer/Camera.hpp>

namespace Develle {

class SceneCamera : public Camera {
public:
  enum class ProjectionType { Perspective = 0, Orthographic = 1 };

  SceneCamera();
  ~SceneCamera() = default;

  void SetPerspective(float verticalFOV, float nearClip, float farClip);
  void SetOrthographic(float size, float nearClip, float farClip);

  void SetViewportSize(uint32_t width, uint32_t height);

  float GetPerspectiveVerticalFOV() const { return perspectiveFOV; }
  void SetPerspectiveVerticalFOV(float verticalFOV) {
    perspectiveFOV = verticalFOV;
  }

  float GetPerspectiveNear() const { return perspectiveNear; }
  void SetPerspectiveNear(float nearClip) { perspectiveNear = nearClip; }
  float GetPerspectiveFar() const { return perspectiveFar; }
  void SetPerspectiveFar(float farClip) { perspectiveFar = farClip; }

  float GetOrthographicNear() const { return orthographicNear; }
  void SetOrthographicNear(float nearClip) { orthographicNear = nearClip; }
  float GetOrthographicFar() const { return orthographicFar; }
  void SetOrthographicFar(float farClip) { orthographicFar = farClip; }

  ProjectionType GetProjectionType() const { return projectionType; }
  void SetProjectionType(ProjectionType type) {
    projectionType = type;
    RecalculateProjection();
  }

private:
  void RecalculateProjection();

  ProjectionType projectionType = ProjectionType::Orthographic;

  float perspectiveFOV = glm::radians(45.0f);
  float perspectiveNear = 0.01f, perspectiveFar = 1000.0f;

  float orthographicSize = 10.0f;
  float orthographicNear = -1.0f, orthographicFar = 1.0f;

  float aspectRatio = 0.0f;
};

} // namespace Develle
#endif // SCENECAMERA_HPP_
