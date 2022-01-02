#ifndef RENDERCOMMAND_H_
#define RENDERCOMMAND_H_

#include "Pipeline.hpp"
#include "RendererAPI.hpp"

namespace Develle {

class RenderCommand {
 public:
  static void Init(const PipelineOptions &options) { rendererAPI->Init(options); }

  static void BeginScene() { rendererAPI->BeginScene(); }

  static void EndScene() { rendererAPI->EndScene(); }

  static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    rendererAPI->SetViewport(x, y, width, height);
  }

  static void SetClearColor(const glm::vec4 &color) { rendererAPI->SetClearColor(color); }

  static void Clear() { rendererAPI->Clear(); }

  static void DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t count = 0) {
    rendererAPI->DrawIndexed(vertexArray, count);
  }

 private:
  static Scope<RendererAPI> rendererAPI;
};

}  // namespace Develle
#endif  // RENDERCOMMAND_H_
