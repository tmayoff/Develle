#pragma once
#ifndef SANDBOX3D_HPP_
#define SANDBOX3D_HPP_

#include <Develle.hpp>
#include <Develle/Renderer/EditorCamera.hpp>

class Sandbox3D : public Develle::Layer {
 public:
  Sandbox3D();
  ~Sandbox3D() = default;

  void OnAttach() override;
  void OnDetach() override;

  void OnUpdate(Develle::Timestep delta) override;
  void OnImGuiRender() override;
  void OnEvent(Develle::Event &e) override;

 private:
  Develle::EditorCamera camera;
  Develle::Mesh mesh;
  Develle::Ref<Develle::Texture2D> checkerboard;
};

#endif  // SANDBOX3D_HPP_
