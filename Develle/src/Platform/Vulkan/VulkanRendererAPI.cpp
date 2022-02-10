#include "VulkanRendererAPI.hpp"

#include <gsl/gsl>

#include "VulkanContext.hpp"

namespace Develle {

void VulkanRendererAPI::Init(const PipelineOptions &options) {
  pipelineOptions = options;

  std::vector<vk::DescriptorSetLayoutBinding> descriptorBindings;
  for (auto &descriptorLayout : options.descriptorLayouts) {
    vk::DescriptorSetLayoutBinding descriptorBinding{};
    descriptorBinding.setBinding(descriptorLayout.Binding);
    switch (descriptorLayout.Type) {
      case DescriptorType::UNIFORM_BUFFER:
        descriptorBinding.setDescriptorType(vk::DescriptorType::eUniformBuffer);
        break;
    }
  }

  vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
  descriptorSetLayoutInfo.setBindings(descriptorBindings);
  descriptorSetLayout =
      GetCurrentVulkanContext().GetDevice().createDescriptorSetLayout(descriptorSetLayoutInfo);

  recreateRenderPass();
  recreateFramebuffers();
  recreateCommandBuffers();
}

void VulkanRendererAPI::BeginScene() {
  auto &vulkanContext = GetCurrentVulkanContext();

  auto frame = GetCurrentCommandBuffer();

  vk::Result waitForFence =
      vulkanContext.GetDevice().waitForFences(frame.CommandQueueFence, false, UINT64_MAX);
  DV_CORE_ASSERT(waitForFence == vk::Result::eSuccess, "Failed to wait for fences");
  vulkanContext.GetDevice().resetFences(frame.CommandQueueFence);

  frame.Commands.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));

  auto clearColors = vk::ClearValue(vk::ClearColorValue(clearColor));

  vk::RenderPassBeginInfo renderPassInfo;
  renderPassInfo.setFramebuffer(framebuffers.at(currentFrameIndex))
      .setRenderPass(renderPass)
      .setRenderArea(vk::Rect2D({0, 0}, vulkanContext.GetSurfaceExtent()))
      .setClearValues(clearColors);

  frame.Commands.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
  // frame.Commands.bindDescriptorSets(descriptorSetLayout);
  frame.Commands.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.GetHandle());
  frame.Commands.setViewport(0, viewport);
  frame.Commands.setScissor(0, scissor);
}

void VulkanRendererAPI::EndScene() {
  auto &vulkanContext = GetCurrentVulkanContext();

  auto frame = GetCurrentCommandBuffer();

  frame.Commands.endRenderPass();
  frame.Commands.end();

  try {
    auto acquiredImage = vulkanContext.GetDevice().acquireNextImageKHR(
        vulkanContext.GetSwapchain(), UINT64_MAX, vulkanContext.GetImageAvailableSemaphore());
    presentImageIndex = acquiredImage.value;
  } catch (vk::OutOfDateKHRError &) {
    recreateSwapchain();
    return;
  }

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

  try {
    auto presentSucceeded = vulkanContext.GetPresentQueue().presentKHR(presentInfo);
    DV_CORE_ASSERT(presentSucceeded == vk::Result::eSuccess, "Failed to present");
  } catch (vk::OutOfDateKHRError &) {
    recreateSwapchain();
  }

  vulkanContext.GetGraphicsQueue().waitIdle();

  currentFrameIndex = (currentFrameIndex + 1) % commandBuffers.size();
}

void VulkanRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t height, uint32_t width) {
  viewport = vk::Viewport((float)x, (float)y, (float)width, (float)height);
  scissor = vk::Rect2D({0, 0}, {width, height});
}

void VulkanRendererAPI::SetClearColor(const glm::vec4 &color) {
  clearColor = {color.r, color.g, color.b, color.a};
}

void VulkanRendererAPI::Clear() {}

void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray> &, uint32_t indexCount) {
  auto &cmd = GetCurrentCommandBuffer();
  cmd.Commands.drawIndexed(indexCount, 1, 0, 0, 0);
}

void VulkanRendererAPI::recreateSwapchain() {
  auto &vulkanContext = GetCurrentVulkanContext();

  vulkanContext.GetDevice().waitIdle();

  vulkanContext.RecreateSwapchain(gsl::narrow_cast<float>(viewport.width),
                                  gsl::narrow_cast<float>(viewport.height));
  recreateRenderPass();
  recreateFramebuffers();
  recreateCommandBuffers();
}

void VulkanRendererAPI::recreateRenderPass() {
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
  pipeline = VulkanPipeline(pipelineOptions, renderPass);
}

void VulkanRendererAPI::recreateFramebuffers() {
  auto &vulkanContext = GetCurrentVulkanContext();

  framebuffers.clear();
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
}

void VulkanRendererAPI::recreateCommandBuffers() {
  auto &vulkanContext = GetCurrentVulkanContext();
  vk::CommandBufferAllocateInfo allocInfo;
  allocInfo.setCommandPool(vulkanContext.GetCommandPool())
      .setCommandBufferCount(framebuffers.size())
      .setLevel(vk::CommandBufferLevel::ePrimary);

  auto allocedCommandBuffers = vulkanContext.GetDevice().allocateCommandBuffers(allocInfo);

  commandBuffers.clear();
  commandBuffers.resize(framebuffers.size());
  for (size_t i = 0; i < framebuffers.size(); i++) {
    auto fence = vulkanContext.GetDevice().createFence(
        vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
    commandBuffers.at(i) = CommandBuffer{allocedCommandBuffers[i], fence};
  }
}

}  // namespace Develle
