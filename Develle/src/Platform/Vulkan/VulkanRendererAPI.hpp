#ifndef VULKANRENDERERAPI_HPP_
#define VULKANRENDERERAPI_HPP_

#include <Develle/Renderer/RendererAPI.hpp>

namespace Develle {

class VulkanRendererAPI : public RendererAPI {
 public:
  void Init() override;
  void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

  void SetClearColor(const glm::vec4& color) override;
  void Clear() override;

  void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
};

}  // namespace Develle

#endif  // VULKANRENDERERAPI_HPP_
