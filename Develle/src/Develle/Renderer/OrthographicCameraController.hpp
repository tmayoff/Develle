#ifndef ORTHOGRAPHICCAMERACONTROLLER_HPP_
#define ORTHOGRAPHICCAMERACONTROLLER_HPP_

#include <Develle/Core/Timestep.hpp>
#include <Develle/Events/ApplicationEvent.hpp>
#include <Develle/Events/MouseEvent.hpp>
#include <Develle/Renderer/OrthographicCamera.hpp>

namespace Develle {

class OrthographicCameraController {
public:
  OrthographicCameraController(float aspectRatio, bool rotation = false);

  void OnUpdate(Timestep delta);
  void OnEvent(Event &e);

  void OnResize(float width, float height);

  OrthographicCamera &GetCamera() { return camera; }
  const OrthographicCamera &GetCamera() const { return camera; }

  float GetZoomLevel() const { return zoomLevel; }
  void SetZoomLevel(float level) { zoomLevel = level; }

private:
  bool OnMouseScrolled(MouseScrolledEvent &e);
  bool OnWindowResized(WindowResizeEvent &e);

  float aspectRation;
  float zoomLevel = 1.0f;
  bool rotation;

  OrthographicCamera camera;

  glm::vec3 cameraPosition = {0.0f, 0.0f, 0.0f};
  float cameraRotation = 0.0f;
  float cameraMoveSpeed = 5.0f;
  float cameraRotationSpeed = 180.0f;
};

} // namespace Develle
#endif // ORTHOGRAPHICCAMERACONTROLLER_HPP_
