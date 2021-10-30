#ifndef SANDBOX2D_HPP_
#define SANDBOX2D_HPP_

#include <Develle.h>

class Sandbox2D : public Develle::Layer {
public:
  Sandbox2D();
  ~Sandbox2D() = default;

  void OnAttach() override;
  void OnDetach() override;

  void OnUpdate(Develle::Timestep delta) override;
  void OnImGuiRender() override;
  void OnEvent(Develle::Event &e) override;

private:
  Develle::OrthographicCameraController cameraController;

  glm::vec4 squareColor = {0.2f, 0.3f, 0.8f, 1.0f};
  Develle::Ref<Develle::Texture2D> checkerboardTexture;
};

#endif // SANDBOX2D_HPP_
