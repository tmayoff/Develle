#pragma once
#ifndef VULKANBUFFER_HPP_
#define VULKANBUFFER_HPP_

#include <vk_mem_alloc.h>

#include <Develle/Renderer/Buffer.hpp>
#include <Develle/Renderer/Pipeline.hpp>
#include <vulkan/vulkan.hpp>

namespace Develle {

class VulkanVertexBuffer : public VertexBuffer {
 public:
  VulkanVertexBuffer(uint32_t size);
  VulkanVertexBuffer(float *vertices, uint32_t size);

  VulkanVertexBuffer(const VulkanVertexBuffer &) = delete;
  VulkanVertexBuffer(VulkanVertexBuffer &&) = delete;
  VulkanVertexBuffer &operator=(VulkanVertexBuffer &&) = delete;
  VulkanVertexBuffer &operator=(const VulkanVertexBuffer &) = delete;

  ~VulkanVertexBuffer() override;

  void Bind() const override;
  void Unbind() const override;

  void SetData(const void *data, uint32_t size) override;

  const VertexBufferLayout &GetLayout() const override { return layout; }
  void SetLayout(const VertexBufferLayout &layout) override { this->layout = layout; }

 private:
  vk::Buffer buffer;
  VertexBufferLayout layout;
  VmaAllocation allocation = {};
};

class VulkanIndexBuffer : public IndexBuffer {
 public:
  VulkanIndexBuffer(uint32_t *indices, uint32_t count);

  VulkanIndexBuffer(const VulkanIndexBuffer &) = delete;
  VulkanIndexBuffer(VulkanIndexBuffer &&) = delete;
  VulkanIndexBuffer &operator=(VulkanIndexBuffer &&) = delete;
  VulkanIndexBuffer &operator=(const VulkanIndexBuffer &) = delete;

  ~VulkanIndexBuffer() override;

  void Bind() const override;
  void Unbind() const override;

  uint32_t GetCount() const override { return count; }

 private:
  vk::Buffer buffer;
  VmaAllocation allocation = {};

  uint32_t count;
};

}  // namespace Develle
#endif  // VULKANBUFFER_HPP_
