#ifndef OPENGLBUFFER_HPP_
#define OPENGLBUFFER_HPP_

#include <Develle/Renderer/Buffer.hpp>

#include "Develle/Renderer/Pipeline.hpp"

namespace Develle {

class OpenGLVertexBuffer : public VertexBuffer {
 public:
  OpenGLVertexBuffer(uint32_t size);
  OpenGLVertexBuffer(float *vertices, uint32_t size);
  ~OpenGLVertexBuffer() override;

  void Bind() const override;
  void Unbind() const override;

  void SetData(const void *data, uint32_t size) override;

  const VertexBufferLayout &GetLayout() const override { return layout; }
  void SetLayout(const VertexBufferLayout &layout) override { this->layout = layout; }

 private:
  uint32_t rendererID;
  VertexBufferLayout layout;
};

class OpenGLIndexBuffer : public IndexBuffer {
 public:
  OpenGLIndexBuffer(uint32_t *indices, uint32_t count);
  ~OpenGLIndexBuffer();

  void Bind() const;
  void Unbind() const;

  uint32_t GetCount() const { return count; }

 private:
  uint32_t rendererID;
  uint32_t count;
};

}  // namespace Develle
#endif  // OPENGLBUFFER_HPP_
