#include <Develle/Renderer/EditorCamera.hpp>

#include <Develle/Core/Input.hpp>
#include <Develle/Core/KeyCodes.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Develle {

EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip,
                           float farClip)
    : fov(fov), aspectRatio(aspectRatio), nearClip(nearClip), farClip(farClip) {
  UpdateView();
}

void EditorCamera::OnUpdate(Timestep) {
  if (Input::IsKeyPressed(Key::LALT)) {
    const glm::vec2 &mouse{Input::GetMouseX(), Input::GetMouseY()};
    glm::vec2 mouseDelta = (mouse - initialMousePosition) * 0.003f;
    initialMousePosition = mouse;

    if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
      MousePan(mouseDelta);
    else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
      MouseRotate(mouseDelta);
    else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
      MouseZoom(mouseDelta.y);
  }

  UpdateView();
}

void EditorCamera::OnEvent(Event &e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<MouseScrolledEvent>(
      DV_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
}

glm::vec3 EditorCamera::GetUpDirection() const {
  return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 EditorCamera::GetRightDirection() const {
  return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 EditorCamera::GetForwardDirection() const {
  return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::quat EditorCamera::GetOrientation() const {
  return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
}

void EditorCamera::UpdateProjection() {
  aspectRatio = viewportWidth / viewportHeight;
  projection =
      glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
}

void EditorCamera::UpdateView() {
  // yaw = pitch = 0.0f // Lock the camera's rotation
  position = CalculatePosition();

  glm::quat orientation = GetOrientation();
  viewMatrix =
      glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
  viewMatrix = glm::inverse(viewMatrix);
}

bool EditorCamera::OnMouseScrolled(MouseScrolledEvent &e) {
  float delta = e.GetYOffset() * 0.1f;
  MouseZoom(delta);
  UpdateView();
  return false;
}

void EditorCamera::MousePan(const glm::vec2 &delta) {
  auto [xSpeed, ySpeed] = PanSpeed();
  focalPoint += -GetRightDirection() * delta.x * xSpeed * distance;
  focalPoint += GetUpDirection() * delta.y * ySpeed * distance;
}

void EditorCamera::MouseRotate(const glm::vec2 &delta) {
  float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
  yaw += yawSign * delta.x * RotationSpeed();
  pitch += delta.y * RotationSpeed();
}

void EditorCamera::MouseZoom(float delta) {
  distance -= delta * ZoomSpeed();
  if (distance < 1.0f) {
    focalPoint += GetForwardDirection();
    distance = 1.0f;
  }
}

glm::vec3 EditorCamera::CalculatePosition() const {
  return focalPoint - GetForwardDirection() * distance;
}

std::pair<float, float> EditorCamera::PanSpeed() const {
  float x = std::min(viewportWidth / 1000.0f, 2.4f); // max = 2.4f
  float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

  float y = std::min(viewportHeight / 1000.0f, 2.4f); // max = 2.4f
  float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

  return {xFactor, yFactor};
}

float EditorCamera::RotationSpeed() const { return 0.8f; }

float EditorCamera::ZoomSpeed() const {
  float distance = this->distance * 0.2f;
  distance = std::max(distance, 0.0f);
  float speed = distance * distance;
  speed = std::min(speed, 100.0f); // max = 100.0f

  return speed;
}

} // namespace Develle
