#include <Develle/Renderer/Buffer.hpp>

#include <Platform/OpenGL/OpenGLBuffer.hpp>

namespace Develle {

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
  // TODO(tyler) Multiple rendering APIs
  return CreateRef<OpenGLVertexBuffer>(size);
}

Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size) {
  // TODO(tyler) Multiple rendering APIs
  return CreateRef<OpenGLVertexBuffer>(vertices, size);
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t count) {
  // TODO(tyler) Multiple rendering APIs
  return CreateRef<OpenGLIndexBuffer>(indices, count);
}

} // namespace Develle
