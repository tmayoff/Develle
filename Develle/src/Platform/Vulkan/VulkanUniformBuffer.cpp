#include "VulkanUniformBuffer.hpp"

#include "VulkanContext.hpp"

namespace Develle {

VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size, uint32_t binding) {
  auto& vulkanContext = GetCurrentVulkanContext();

  vk::BufferCreateInfo bufferInfo{};
  bufferInfo.setSize(vk::DeviceSize(size))
      .setUsage(vk::BufferUsageFlagBits::eUniformBuffer)
      .setSharingMode(vk::SharingMode::eExclusive);

  VmaAllocationCreateInfo allocInfo = {};
  allocInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_TO_CPU;
  vmaCreateBuffer(vulkanContext.GetAllocator(), (VkBufferCreateInfo*)&bufferInfo, &allocInfo,
                  (VkBuffer*)&buffer, &allocation, nullptr);
}

VulkanUniformBuffer::~VulkanUniformBuffer() {
  auto& vulkanContext = GetCurrentVulkanContext();
  vmaDestroyBuffer(vulkanContext.GetAllocator(), buffer, allocation);
}

void VulkanUniformBuffer::SetData(const void* data, uint32_t size, uint32_t) {
  // TODO offsets

  void* mem = nullptr;
  vmaMapMemory(GetCurrentVulkanContext().GetAllocator(), allocation, &mem);
  std::memcpy(mem, data, size);
  vmaUnmapMemory(GetCurrentVulkanContext().GetAllocator(), allocation);
}

}  // namespace Develle
