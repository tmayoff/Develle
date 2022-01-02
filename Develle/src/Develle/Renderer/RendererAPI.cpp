#include "RendererAPI.hpp"

#include <Platform/OpenGL/OpenGLRendererAPI.hpp>

namespace Develle {

RendererAPI::API RendererAPI::api = RendererAPI::API::Vulkan;

Scope<RendererAPI> RendererAPI::Create() {
  // TODO(tyler) Add more rendering apis
  switch (api) {
    case RendererAPI::API::OpenGL:
      return CreateScope<OpenGLRendererAPI>();
    default:
      break;
  }
}

}  // namespace Develle
