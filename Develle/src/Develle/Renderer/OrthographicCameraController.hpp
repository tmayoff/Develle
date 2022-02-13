#ifndef ORTHOGRAPHICCAMERACONTROLLER_HPP_
#define ORTHOGRAPHICCAMERACONTROLLER_HPP_

#include <Develle/Core/Timestep.hpp>
#include <Develle/Events/ApplicationEvent.hpp>
#include <Develle/Events/MouseEvent.hpp>

#include "Camera.hpp"

namespace Develle {

class OrthographicCameraController {
 public:
  OrthographicCameraController(float aspectRatio, bool rotation = false);

  void OnUpdate(Timestep delta);
  void OnEvent(Event &e);

  void OnResize(float width, float height);

  Camera &GetCamera() { return camera; }
  const Camera &GetCamera() const { return camera; }

  float GetZoomLevel() const { return zoomLevel; }
  void SetZoomLevel(float level) { zoomLevel = level; }

 private:
  bool OnMouseScrolled(MouseScrolledEvent &e);
  bool OnWindowResized(WindowResizeEvent &e);

  float aspectRation;
  float zoomLevel = 1.0f;
  bool rotation;

  Camera camera;

  glm::vec3 cameraPosition = glm::vec3(0.0f);

  float cameraRotation = 0.0f;         // NOLINT
  float cameraMoveSpeed = 5.0f;        // NOLINT
  float cameraRotationSpeed = 180.0f;  // NOLINT
};

}  // namespace Develle
#endif  // ORTHOGRAPHICCAMERACONTROLLER_HPP_
