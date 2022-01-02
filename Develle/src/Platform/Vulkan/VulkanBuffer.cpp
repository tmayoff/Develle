#include "VulkanBuffer.hpp"

#include "VulkanContext.hpp"

namespace Develle {

VulkanVertexBuffer::VulkanVertexBuffer(float *vertices, uint32_t size) {
  auto &vulkanContext = GetCurrentVulkanContext();

  vk::BufferCreateInfo bufferInfo{};
  bufferInfo.setSize(vk::DeviceSize(size))
      .setUsage(vk::BufferUsageFlagBits::eVertexBuffer)
      .setSharingMode(vk::SharingMode::eExclusive);

  buffer = vulkanContext.GetDevice().createBuffer(bufferInfo);

  VmaAllocationCreateInfo allocInfo = {};
  allocInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_TO_CPU;
  vmaCreateBuffer(vulkanContext.GetAllocator(), (VkBufferCreateInfo *)&bufferInfo, &allocInfo,
                  (VkBuffer *)&buffer, &allocation, nullptr);

  SetData(vertices, size);
}

VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size) {
  auto &vulkanContext = GetCurrentVulkanContext();

  vk::BufferCreateInfo bufferInfo{};
  bufferInfo.setSize(vk::DeviceSize(size))
      .setUsage(vk::BufferUsageFlagBits::eVertexBuffer)
      .setSharingMode(vk::SharingMode::eExclusive);

  buffer = vulkanContext.GetDevice().createBuffer(bufferInfo);

  VmaAllocationCreateInfo allocInfo = {};
  allocInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_TO_CPU;
  vmaCreateBuffer(vulkanContext.GetAllocator(), (VkBufferCreateInfo *)&bufferInfo, &allocInfo,
                  (VkBuffer *)&buffer, &allocation, nullptr);
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
  auto &vulkanContext = GetCurrentVulkanContext();
  vmaDestroyBuffer(vulkanContext.GetAllocator(), buffer, allocation);
}

void VulkanVertexBuffer::Bind() const {}

void VulkanVertexBuffer::Unbind() const {}

void VulkanVertexBuffer::SetData(const void *data, uint32_t size) {
  void *mem = nullptr;
  vmaMapMemory(GetCurrentVulkanContext().GetAllocator(), allocation, &mem);
  std::memcpy(mem, data, size);
  vmaUnmapMemory(GetCurrentVulkanContext().GetAllocator(), allocation);
}

// ============= Index Buffer ============= //

VulkanIndexBuffer::VulkanIndexBuffer(uint32_t *indices, uint32_t count) {
  auto &vulkanContext = GetCurrentVulkanContext();

  vk::BufferCreateInfo bufferInfo{};
  bufferInfo.setSize(vk::DeviceSize(sizeof(uint32_t) * count))
      .setUsage(vk::BufferUsageFlagBits::eIndexBuffer)
      .setSharingMode(vk::SharingMode::eExclusive);

  buffer = vulkanContext.GetDevice().createBuffer(bufferInfo);

  VmaAllocationCreateInfo allocInfo = {};
  allocInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_TO_CPU;
  vmaCreateBuffer(vulkanContext.GetAllocator(), (VkBufferCreateInfo *)&bufferInfo, &allocInfo,
                  (VkBuffer *)&buffer, &allocation, nullptr);
}

VulkanIndexBuffer::~VulkanIndexBuffer() {
  auto &vulkanContext = GetCurrentVulkanContext();
  vmaDestroyBuffer(vulkanContext.GetAllocator(), buffer, allocation);
}

void VulkanIndexBuffer::Bind() const {}

void VulkanIndexBuffer::Unbind() const {}

}  // namespace Develle
