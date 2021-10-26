#include <Develle/Renderer/Renderer.hpp>

namespace Develle {

void Renderer::Init() {
  DV_PROFILE_FUNCTION();

  RenderCommand::Init();
  //   Renderer2D::Init();
}

void Renderer::Shutdown() {}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {}

void Renderer::BeginScene() {}

void Renderer::EndScene() {}

} // namespace Develle
