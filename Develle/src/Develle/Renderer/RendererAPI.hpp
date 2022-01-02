#ifndef RENDERERAPI_HPP_
#define RENDERERAPI_HPP_

#include <glm/glm.hpp>

#include "Pipeline.hpp"
#include "VertexArray.hpp"

namespace Develle {

class RendererAPI {
 public:
  enum class API { None = 0, OpenGL = 1, Vulkan = 2 };

  virtual ~RendererAPI() = default;

  virtual void Init(const PipelineOptions &options) = 0;
  virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
  virtual void SetClearColor(const glm::vec4 &color) = 0;
  virtual void Clear() = 0;

  virtual void BeginScene() = 0;
  virtual void EndScene() = 0;

  virtual void DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount = 0) = 0;

  static API GetAPI() { return api; }
  static Scope<RendererAPI> Create();

 private:
  static API api;
};

}  // namespace Develle

#endif  // RENDERERAPI_HPP_
