#include "UniformBuffer.hpp"

#include <Platform/OpenGL/OpenGLUniformBuffer.hpp>
#include <Platform/Vulkan/VulkanUniformBuffer.hpp>

#include "RendererAPI.hpp"

namespace Develle {

Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
  switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLUniformBuffer>(size, binding);
    case RendererAPI::API::Vulkan:
      return CreateRef<VulkanUniformBuffer>(size, binding);
    default:
      DV_CORE_ASSERT(false, "Unknown rendering api");
      return nullptr;
  }
}

}  // namespace Develle
