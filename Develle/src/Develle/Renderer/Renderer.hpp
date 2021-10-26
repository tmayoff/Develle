#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <Develle/Renderer/RenderCommand.hpp>

namespace Develle {

class Renderer {
public:
  static void Init();
  static void Shutdown();

  static void OnWindowResize(uint32_t width, uint32_t height);

  static void BeginScene();
  static void EndScene();

  //   static void Submit(const Ref<Shader)

  static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
};

} // namespace Develle
#endif // RENDERER_HPP_
