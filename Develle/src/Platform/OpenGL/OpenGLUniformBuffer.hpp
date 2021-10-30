#ifndef OPENGLUNIFORMBUFFER_HPP_
#define OPENGLUNIFORMBUFFER_HPP_

#include <Develle/Renderer/UniformBuffer.hpp>

namespace Develle {

class OpenGLUniformBuffer : public UniformBuffer {
public:
  OpenGLUniformBuffer(uint32_t size, uint32_t binding);
  ~OpenGLUniformBuffer();

  void SetData(const void *data, uint32_t size, uint32_t offset = 0) override;

private:
  uint32_t rendererID = 0;
};

} // namespace Develle
#endif // OPENGLUNIFORMBUFFER_HPP_
