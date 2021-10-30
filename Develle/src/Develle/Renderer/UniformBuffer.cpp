#include <Develle/Renderer/UniformBuffer.hpp>

#include <Platform/OpenGL/OpenGLUniformBuffer.hpp>

namespace Develle {

Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
  // TODO(tyler) multiple APIs
  return CreateRef<OpenGLUniformBuffer>(size, binding);
}

} // namespace Develle
