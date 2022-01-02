#include "Pipeline.hpp"

#include <Platform/Vulkan/VulkanPipeline.hpp>

#include "RendererAPI.hpp"

namespace Develle {

Scope<Pipeline> Pipeline::Create(const PipelineOptions &options) {
  switch (RendererAPI::GetAPI()) {
    // case RendererAPI::API::OpenGL:
    //   return CreateScope<OpenGLRendererAPI>();
    case RendererAPI::API::Vulkan:
      return CreateScope<VulkanPipeline>(options);
    default:
      return nullptr;
  }
}

}  // namespace Develle
