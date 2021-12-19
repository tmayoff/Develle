#include "RendererAPI.hpp"

#include <Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <Platform/Vulkan/VulkanRendererAPI.hpp>

namespace Develle {

RendererAPI::API RendererAPI::api = RendererAPI::API::Vulkan;

Scope<RendererAPI> RendererAPI::Create() {
  // TODO(tyler) Add more rendering apis
  switch (api) {
    case RendererAPI::API::OpenGL:
      return CreateScope<OpenGLRendererAPI>();
    case RendererAPI::API::Vulkan:
      return CreateScope<VulkanRendererAPI>();
    default:
      break;
  }
}

}  // namespace Develle
