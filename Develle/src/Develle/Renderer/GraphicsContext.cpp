#include "GraphicsContext.hpp"

#include <Platform/OpenGL/OpenGLContext.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>

#include "RendererAPI.hpp"

namespace Develle {

Scope<GraphicsContext> GraphicsContext::Create(void *window) {
  switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return CreateScope<OpenGLGraphicsContext>(static_cast<SDL_Window *>(window));
    case RendererAPI::API::Vulkan: {
      VulkanContextCreateOptions options{};
      options.Layers = {"VK_LAYER_KHRONOS_validation" /*, "VK_LAYER_RENDERDOC_Capture"*/};
      options.Extensions = VulkanContext::GetRequiredExtensions(static_cast<SDL_Window *>(window));
      auto vulkanContext = CreateScope<VulkanContext>(static_cast<SDL_Window *>(window), options);
      SetCurrentVulkanContext(*vulkanContext.get());
      return vulkanContext;
    }
    default:
      DV_CORE_ASSERT(false, "Unknown Renderer API");
      return nullptr;
  }
}

}  // namespace Develle
