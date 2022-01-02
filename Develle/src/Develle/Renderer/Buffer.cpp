#include "Buffer.hpp"

#include <Platform/OpenGL/OpenGLBuffer.hpp>
#include <Platform/Vulkan/VulkanBuffer.hpp>

#include "RendererAPI.hpp"
namespace Develle {

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
  switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLVertexBuffer>(size);
    case RendererAPI::API::Vulkan:
      return CreateRef<VulkanVertexBuffer>(size);
    default:
      return nullptr;
  }
}

Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size) {
  switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLVertexBuffer>(vertices, size);
    case RendererAPI::API::Vulkan:
      return CreateRef<VulkanVertexBuffer>(vertices, size);
    default:
      return nullptr;
  }
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t count) {
  switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLIndexBuffer>(indices, count);
    case RendererAPI::API::Vulkan:
      return CreateRef<VulkanIndexBuffer>(indices, count);
    default:
      return nullptr;
  }
}

}  // namespace Develle
