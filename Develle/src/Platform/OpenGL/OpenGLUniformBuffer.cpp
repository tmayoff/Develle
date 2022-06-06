#include <glad/glad.h>

#include <Platform/OpenGL/OpenGLUniformBuffer.hpp>

namespace Develle {

OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
  glCreateBuffers(1, &rendererID);
  glNamedBufferData(rendererID, size, nullptr, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, binding, rendererID);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer() { glDeleteBuffers(1, &rendererID); }

void OpenGLUniformBuffer::SetData(const void *data, uint32_t size, uint32_t offset) {
  glNamedBufferSubData(rendererID, offset, size, data);
}
}  // namespace Develle
