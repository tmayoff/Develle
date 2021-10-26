#ifndef OPENGLVERTEXARRAY_HPP_
#define OPENGLVERTEXARRAY_HPP_

#include <Develle/Renderer/VertexArray.hpp>

namespace Develle {

class OpenGLVertexArray : public VertexArray {
public:
  OpenGLVertexArray();
  ~OpenGLVertexArray();

  void Bind() const override;
  void Unbind() const override;

  void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;
  void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) override;

  const std::vector<Ref<VertexBuffer>> &GetVertexBuffers() const override {
    return vertexBuffers;
  }

  const Ref<IndexBuffer> &GetIndexBuffer() const override {
    return indexBuffer;
  }

private:
  uint32_t rendererID;
  uint32_t vertexBufferIndex = 0;
  std::vector<Ref<VertexBuffer>> vertexBuffers;
  Ref<IndexBuffer> indexBuffer;
};

} // namespace Develle
#endif // OPENGLVERTEXARRAY_HPP_
