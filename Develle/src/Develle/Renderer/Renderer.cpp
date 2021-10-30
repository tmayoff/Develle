#include <Develle/Renderer/Renderer.hpp>
#include <Develle/Renderer/Renderer2D.hpp>

namespace Develle {

void Renderer::Init() {
  DV_PROFILE_FUNCTION();

  RenderCommand::Init();
  Renderer2D::Init();
}

void Renderer::Shutdown() { Renderer2D::Shutdown(); }

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
  RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene() {}

void Renderer::EndScene() {}

} // namespace Develle
