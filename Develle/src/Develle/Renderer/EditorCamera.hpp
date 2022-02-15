#ifndef EDITORCAMERA_HPP_
#define EDITORCAMERA_HPP_

#include <Develle/Core/Timestep.hpp>
#include <Develle/Events/Event.hpp>
#include <Develle/Events/MouseEvent.hpp>
#include <Develle/Renderer/Camera.hpp>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Develle {

class EditorCamera {
 public:
  EditorCamera() = default;

  /// @brief Construct a new Editor Camera object
  ///
  /// @param fov In degrees
  /// @param aspectRatio
  /// @param nearClip
  /// @param farClip
  EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

  void OnUpdate(Timestep delta);
  void OnEvent(Event& e);

  bool IsUsing() { return isUsing; }

  inline Camera& GetCamera() { return camera; }

  inline float GetDistance() const { return distance; }
  inline void SetDistance(float distance) { this->distance = distance; }

  inline void SetViewportSize(float width, float height) {
    viewportWidth = width;
    viewportHeight = height;
    UpdateProjection();
  }

  glm::vec3 GetUpDirection() const;
  glm::vec3 GetRightDirection() const;
  glm::vec3 GetForwardDirection() const;
  glm::quat GetOrientation() const;

  // float GetPitch() const { return pitch; }
  // float GetYaw() const { return yaw; }

 private:
  void UpdateProjection();
  void UpdateView();

  bool OnMouseScrolled(MouseScrolledEvent& e);

  void MousePan(const glm::vec2& delta);
  void MouseRotate(const glm::vec2& delta);
  void MouseZoom(float delta);

  glm::vec3 CalculatePosition() const;

  std::pair<float, float> PanSpeed() const;
  float RotationSpeed() const;
  float ZoomSpeed() const;

  Camera camera;

  bool isUsing = false;

  float fov = 45.0f;           // NOLINT
  float aspectRatio = 1.778f;  // NOLINT
  float nearClip = 0.001f;     // NOLINT
  float farClip = 1000.0f;     // NOLINT

  glm::vec3 cameraPosition = glm::vec3{0.0f, 0.0f, 10.0f};  // NOLINT
  glm::vec3 focalPoint = {0.0f, 0.0f, 0.0f};

  glm::vec2 initialMousePosition = {0.0f, 0.0f};

  float distance = 10.0f;  // NOLINT
  float pitch = 0.0f, yaw = 0.0f;

  float viewportWidth = 1280;  // NOLINT
  float viewportHeight = 720;  // NOLINT
};

}  // namespace Develle
#endif  // EDITORCAMERA_HPP_
