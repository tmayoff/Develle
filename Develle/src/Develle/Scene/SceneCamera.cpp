#include <Develle/Scene/SceneCamera.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Develle {

SceneCamera::SceneCamera() { RecalculateProjection(); }

void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip) {
  projectionType = ProjectionType::Perspective;
  perspectiveFOV = verticalFOV;
  perspectiveNear = nearClip;
  perspectiveFar = farClip;
  RecalculateProjection();
}

void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
  projectionType = ProjectionType::Perspective;
  orthographicSize = size;
  orthographicNear = nearClip;
  orthographicFar = farClip;
  RecalculateProjection();
}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
  DV_CORE_ASSERT(width > 0 && height > 0, "Can't set viewport size to 0");
  aspectRatio = (float)width / (float)height;
}

void SceneCamera::RecalculateProjection() {
  // switch (projectionType) {
  // case ProjectionType::Perspective:
  //   projection = glm::perspective(perspectiveFOV, aspectRatio, perspectiveNear,
  //                                 perspectiveFar);
  //   break;
  // case ProjectionType::Orthographic:
  //   float orthoLeft = -orthographicSize * aspectRatio * 0.5f;
  //   float orthoRight = orthographicSize * aspectRatio * 0.5f;
  //   float orthoBottom = -orthographicSize * 0.5f;
  //   float orthoTop = orthographicSize * 0.5f;

  //   projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
  //                           orthographicNear, orthographicFar);
  //   break;
  // }
}

}  // namespace Develle
