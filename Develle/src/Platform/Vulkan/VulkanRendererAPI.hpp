#pragma once
#ifndef VULKANRENDERERAPI_HPP_
#define VULKANRENDERERAPI_HPP_

#include <Develle/Renderer/RendererAPI.hpp>
#include <vulkan/vulkan.hpp>

#include "VulkanPipeline.hpp"

namespace Develle {

class VulkanRendererAPI : public RendererAPI {
 public:
  VulkanRendererAPI() = default;

  void Init(const PipelineOptions &options) override;

  void BeginScene() override;
  void EndScene() override;

  void SetViewport(uint32_t x, uint32_t y, uint32_t height, uint32_t width) override;

  void SetClearColor(const glm::vec4 &color) override;
  void Clear() override;

  void DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount = 0) override;

 private:
  VulkanPipeline pipeline;
  vk::RenderPass renderPass;
  vk::PipelineLayout layout;
};

}  // namespace Develle
#endif  // VULKANRENDERERAPI_HPP_
