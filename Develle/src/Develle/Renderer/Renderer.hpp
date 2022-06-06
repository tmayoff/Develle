#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <Develle/Renderer/RenderCommand.hpp>

namespace Develle {

enum class RenderMode { Render3D, Render2D };

class Renderer {
 public:
  static void Init(RenderMode mode);
  static void Shutdown();

  static void OnWindowResize(uint32_t width, uint32_t height);

  static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
};

}  // namespace Develle
#endif  // RENDERER_HPP_
