#pragma once
#ifndef VULKANUNIFORMBUFFER_HPP_
#define VULKANUNIFORMBUFFER_HPP_

#include <vk_mem_alloc.h>

#include <Develle/Renderer/UniformBuffer.hpp>
#include <vulkan/vulkan.hpp>

namespace Develle {

class VulkanUniformBuffer : public UniformBuffer {
 public:
  VulkanUniformBuffer(uint32_t size, uint32_t binding);
  ~VulkanUniformBuffer();

  void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

 private:
  vk::Buffer buffer;
  VmaAllocation allocation = {};
};

}  // namespace Develle
#endif  // VULKANUNIFORMBUFFER_HPP_
