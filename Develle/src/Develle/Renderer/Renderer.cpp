#include <Develle/Renderer/Renderer.hpp>
#include <Develle/Renderer/Renderer2D.hpp>

namespace Develle {

void Renderer::Init(RenderMode mode) {
  DV_PROFILE_FUNCTION();  // NOLINT

  RenderCommand::Init();
  if (mode == RenderMode::Render2D)
    Renderer2D::Init();
  else if (mode == RenderMode::Render3D)
    Renderer3D::Init();
}

void Renderer::Shutdown() { Renderer2D::Shutdown(); }

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
  RenderCommand::SetViewport(0, 0, width, height);
}

}  // namespace Develle
