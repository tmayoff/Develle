#include "VulkanRendererAPI.hpp"

#include "VulkanContext.hpp"

namespace Develle {

void VulkanRendererAPI::Init(const PipelineOptions &options) {
  auto &context = GetCurrentVulkanContext();

  vk::AttachmentDescription colorAttachment{};
  colorAttachment.setFormat(context.GetSurfaceFormat().format)
      .setSamples(vk::SampleCountFlagBits::e1)
      .setLoadOp(vk::AttachmentLoadOp::eClear)
      .setStoreOp(vk::AttachmentStoreOp::eStore)
      .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
      .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
      .setInitialLayout(vk::ImageLayout::eUndefined)
      .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

  vk::AttachmentReference colorRef{};
  colorRef.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

  vk::SubpassDependency dependency{};
  dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL)
      .setDstSubpass(0)
      .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
      .setSrcAccessMask(vk::AccessFlagBits::eMemoryRead)
      .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
      .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

  vk::SubpassDescription subpass{};
  subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics).setColorAttachments(colorRef);

  vk::RenderPassCreateInfo renderPassInfo;
  renderPassInfo.setAttachments(colorAttachment).setSubpasses(subpass).setDependencies(dependency);

  renderPass = context.GetDevice().createRenderPass(renderPassInfo);

  pipeline = VulkanPipeline(options, renderPass);
}

void VulkanRendererAPI::BeginScene() {}

void VulkanRendererAPI::EndScene() {}

void VulkanRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t height, uint32_t width) {}

void VulkanRendererAPI::SetClearColor(const glm::vec4 &color) {}

void VulkanRendererAPI::Clear() {}

void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount) {}

}  // namespace Develle
