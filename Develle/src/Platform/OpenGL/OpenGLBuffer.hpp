#ifndef OPENGLBUFFER_HPP_
#define OPENGLBUFFER_HPP_

#include <Develle/Renderer/Buffer.hpp>

namespace Develle {

class OpenGLVertexBuffer : public VertexBuffer {
 public:
  explicit OpenGLVertexBuffer(uint32_t size);
  OpenGLVertexBuffer(float *vertices, uint32_t size);
  ~OpenGLVertexBuffer() override;

  void Bind() const override;
  void Unbind() const override;

  void SetData(const void *data, uint32_t size) override;

  [[nodiscard]] auto GetLayout() const -> const BufferLayout & override { return layout; }
  void SetLayout(const BufferLayout &layout) override { this->layout = layout; }

 private:
  uint32_t rendererID;
  BufferLayout layout;
};

class OpenGLIndexBuffer : public IndexBuffer {
 public:
  OpenGLIndexBuffer(uint32_t *indices, uint32_t count);
  ~OpenGLIndexBuffer() override;

  void Bind() const override;
  void Unbind() const override;

  [[nodiscard]] auto GetCount() const -> uint32_t override { return count; }

 private:
  uint32_t rendererID;
  uint32_t count;
};

}  // namespace Develle
#endif  // OPENGLBUFFER_HPP_
