#include <Develle/Renderer/RendererAPI.hpp>
#include <Platform/OpenGL/OpenGLRendererAPI.hpp>

namespace Develle {

RendererAPI::API RendererAPI::api = RendererAPI::API::OpenGL;

Scope<RendererAPI> RendererAPI::Create() {
  // TODO(tyler) Add more rendering apis
  return CreateScope<OpenGLRendererAPI>();
}

}  // namespace Develle
