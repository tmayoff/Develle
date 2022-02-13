#include <Develle/Core/Input.hpp>
#include <Develle/Renderer/OrthographicCameraController.hpp>

namespace Develle {

OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
    : aspectRation(aspectRatio),
      rotation(rotation),
      camera(glm::ortho(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel)) {
}

void OrthographicCameraController::OnUpdate(Timestep delta) {
  DV_PROFILE_FUNCTION();  // NOLINT

  if (Input::IsKeyPressed(Key::A)) {
    cameraPosition.x -= cos(glm::radians(cameraRotation)) * cameraMoveSpeed * delta;
    cameraPosition.y -= sin(glm::radians(cameraRotation)) * cameraMoveSpeed * delta;
  } else if (Input::IsKeyPressed(Key::D)) {
    cameraPosition.x += cos(glm::radians(cameraRotation)) * cameraMoveSpeed * delta;
    cameraPosition.y += sin(glm::radians(cameraRotation)) * cameraMoveSpeed * delta;
  }

  if (Input::IsKeyPressed(Key::W)) {
    cameraPosition.x += sin(glm::radians(cameraRotation)) * cameraMoveSpeed * delta;
    cameraPosition.y += cos(glm::radians(cameraRotation)) * cameraMoveSpeed * delta;
  } else if (Input::IsKeyPressed(Key::S)) {
    cameraPosition.x -= sin(glm::radians(cameraRotation)) * cameraMoveSpeed * delta;
    cameraPosition.y -= cos(glm::radians(cameraRotation)) * cameraMoveSpeed * delta;
  }

  if (rotation) {
    if (Input::IsKeyPressed(Key::Q))
      cameraRotation += cameraRotationSpeed * delta;
    else if (Input::IsKeyPressed(Key::E))
      cameraRotation -= cameraRotationSpeed * delta;

    if (cameraRotation > 180.0f)
      cameraRotation -= 360.0f;
    else if (cameraRotation <= -180.0f)
      cameraRotation += 360.0f;
  }

  camera.SetPosition(cameraPosition);
  cameraMoveSpeed = zoomLevel;
}

void OrthographicCameraController::OnEvent(Event &e) {
  DV_PROFILE_FUNCTION();  // NOLINT

  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<MouseScrolledEvent>(
      DV_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
  dispatcher.Dispatch<WindowResizeEvent>(
      DV_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

void OrthographicCameraController::OnResize(float width, float height) {
  aspectRation = width / height;
  camera.SetProjection(
      glm::ortho(-aspectRation * zoomLevel, aspectRation * zoomLevel, -zoomLevel, zoomLevel));
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent &e) {
  DV_PROFILE_FUNCTION();  // NOLINT

  zoomLevel -= e.GetYOffset() * 0.25f;
  zoomLevel = std::max(zoomLevel, 0.25f);
  camera.SetProjection(
      glm::ortho(-aspectRation * zoomLevel, aspectRation * zoomLevel, -zoomLevel, zoomLevel));
  return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent &e) {
  DV_PROFILE_FUNCTION();

  OnResize((float)e.GetWidth(), (float)e.GetHeight());
  return false;
}
}  // namespace Develle
