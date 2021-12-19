#include "VulkanRendererAPI.hpp"

namespace Develle {

void VulkanRendererAPI::Init() {}

void VulkanRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {}

void VulkanRendererAPI::SetClearColor(const glm::vec4& color) {}

void VulkanRendererAPI::Clear() {}

void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {}

}  // namespace Develle
