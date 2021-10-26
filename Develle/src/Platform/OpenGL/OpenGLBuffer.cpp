#include <Platform/OpenGL/OpenGLBuffer.hpp>

#include <glad/glad.h>

namespace Develle {

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
  DV_PROFILE_FUNCTION();

  glCreateBuffers(1, &rendererID);
  glBindBuffer(GL_ARRAY_BUFFER, rendererID);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size) {
  DV_PROFILE_FUNCTION();

  glCreateBuffers(1, &rendererID);
  glBindBuffer(GL_ARRAY_BUFFER, rendererID);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
  DV_PROFILE_FUNCTION();

  glDeleteBuffers(1, &rendererID);
}

void OpenGLVertexBuffer::Bind() const {
  DV_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void OpenGLVertexBuffer::Unbind() const {
  DV_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const void *data, uint32_t size) {
  glBindBuffer(GL_ARRAY_BUFFER, rendererID);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count)
    : count(count) {
  DV_PROFILE_FUNCTION();

  glCreateBuffers(1, &rendererID);

  // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
  // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO
  // state
  glBindBuffer(GL_ARRAY_BUFFER, rendererID);
  glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
  DV_PROFILE_FUNCTION();

  glDeleteBuffers(1, &rendererID);
}

void OpenGLIndexBuffer::Bind() const {
  DV_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void OpenGLIndexBuffer::Unbind() const {
  DV_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace Develle
