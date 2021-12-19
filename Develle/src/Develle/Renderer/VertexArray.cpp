#include <Develle/Renderer/VertexArray.hpp>
#include <Platform/OpenGL/OpenGLVertexArray.hpp>

#include "RendererAPI.hpp"

namespace Develle {

Ref<VertexArray> VertexArray::Create() {
  switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLVertexArray>();
    default:
      DV_CORE_ASSERT(false, "Unknown rendering api");
  }
}
}  // namespace Develle
