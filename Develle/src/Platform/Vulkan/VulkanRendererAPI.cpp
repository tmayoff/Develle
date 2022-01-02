#include "VulkanRendererAPI.hpp"

#include "VulkanContext.hpp"

namespace Develle {

void VulkanRendererAPI::Init(const PipelineOptions &options) {
  // Render Pass
  auto &vulkanContext = GetCurrentVulkanContext();

  vk::AttachmentDescription colorAttachment{};
  colorAttachment.setFormat(vulkanContext.GetSurfaceFormat().format)
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

  renderPass = vulkanContext.GetDevice().createRenderPass(renderPassInfo);
  pipeline = VulkanPipeline(options, renderPass);

  //
  framebuffers.resize(vulkanContext.GetSwapchainImageViews().size());
  for (size_t i = 0; i < vulkanContext.GetSwapchainImageViews().size(); i++) {
    std::array attachments = {vulkanContext.GetSwapchainImageViews().at(i)};

    auto extent = vulkanContext.GetSurfaceExtent();

    vk::FramebufferCreateInfo framebufferInfo{};
    framebufferInfo.setRenderPass(renderPass);
    framebufferInfo.setAttachments(attachments);
    framebufferInfo.setWidth(extent.width);
    framebufferInfo.setHeight(extent.height);
    framebufferInfo.setLayers(1);

    framebuffers.at(i) = vulkanContext.GetDevice().createFramebuffer(framebufferInfo);
  }

  vk::CommandBufferAllocateInfo allocInfo;
  allocInfo.setCommandPool(vulkanContext.GetCommandPool())
      .setCommandBufferCount(framebuffers.size())
      .setLevel(vk::CommandBufferLevel::ePrimary);

  auto allocedCommandBuffers = vulkanContext.GetDevice().allocateCommandBuffers(allocInfo);

  for (size_t i = 0; i < framebuffers.size(); i++) {
    auto fence = vulkanContext.GetDevice().createFence(
        vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
    commandBuffers.push_back(CommandBuffer{allocedCommandBuffers[i], fence});
  }
}

void VulkanRendererAPI::BeginScene() {
  auto &vulkanContext = GetCurrentVulkanContext();

  auto acquiredImage = vulkanContext.GetDevice().acquireNextImageKHR(
      vulkanContext.GetSwapchain(), UINT64_MAX, vulkanContext.GetImageAvailableSemaphore());
  presentImageIndex = acquiredImage.value;

  auto frame = GetCurrentCommandBuffer();

  vk::Result waitForFence =
      vulkanContext.GetDevice().waitForFences(frame.CommandQueueFence, false, UINT64_MAX);
  DV_CORE_ASSERT(waitForFence == vk::Result::eSuccess, "Failed to wait for fences");
  vulkanContext.GetDevice().resetFences(frame.CommandQueueFence);

  frame.Commands.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));

  auto clearColors = vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.2f, 0.2f, 0.2f}));

  vk::RenderPassBeginInfo renderPassInfo;
  renderPassInfo.setFramebuffer(framebuffers.at(currentFrameIndex))
      .setRenderPass(renderPass)
      .setRenderArea(vk::Rect2D({0, 0}, vulkanContext.GetSurfaceExtent()))
      .setClearValues(clearColors);

  frame.Commands.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
  frame.Commands.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.GetHandle());
  frame.Commands.setViewport(0, viewport);
}

void VulkanRendererAPI::EndScene() {
  auto &vulkanContext = GetCurrentVulkanContext();
  auto frame = GetCurrentCommandBuffer();

  frame.Commands.endRenderPass();
  frame.Commands.end();

  std::array waitStages = {
      (vk::PipelineStageFlags)vk::PipelineStageFlagBits::eColorAttachmentOutput};

  vk::SubmitInfo submitInfo;
  submitInfo.setWaitSemaphores(vulkanContext.GetImageAvailableSemaphore())
      .setWaitDstStageMask(waitStages)
      .setSignalSemaphores(vulkanContext.GetRenderingFinishedSemaphore())
      .setCommandBuffers(frame.Commands);

  vulkanContext.GetDevice().resetFences(frame.CommandQueueFence);

  vulkanContext.GetGraphicsQueue().submit(submitInfo, frame.CommandQueueFence);

  vk::PresentInfoKHR presentInfo;
  presentInfo.setWaitSemaphores(vulkanContext.GetRenderingFinishedSemaphore())
      .setSwapchains(vulkanContext.GetSwapchain())
      .setImageIndices(presentImageIndex);

  auto presentSucceeed = vulkanContext.GetPresentQueue().presentKHR(presentInfo);
  DV_CORE_ASSERT(presentSucceeed == vk::Result::eSuccess, "Failed to present");

  currentFrameIndex = (currentFrameIndex + 1) % commandBuffers.size();
}

void VulkanRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t height, uint32_t width) {
  viewport = vk::Viewport((float)x, (float)y, (float)width, (float)height);
}

void VulkanRendererAPI::SetClearColor(const glm::vec4 &color) {}

void VulkanRendererAPI::Clear() {}

void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount) {
  auto &cmd = GetCurrentCommandBuffer();
  cmd.Commands.drawIndexed(indexCount, 1, 0, 0, 0);
}

}  // namespace Develle
