#include <Develle/Renderer/VertexArray.hpp>

#include <Platform/OpenGL/OpenGLVertexArray.hpp>

namespace Develle {

Ref<VertexArray> VertexArray::Create() {
  return CreateRef<OpenGLVertexArray>();
}

} // namespace Develle
