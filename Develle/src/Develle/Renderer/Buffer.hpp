#ifndef BUFFER_HPP_
#define BUFFER_HPP_

namespace Develle {

struct BufferLayout;

class VertexBuffer {
 public:
  virtual ~VertexBuffer() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void SetData(const void *data, uint32_t size) = 0;

  virtual const BufferLayout &GetLayout() const = 0;
  virtual void SetLayout(const BufferLayout &layout) = 0;

  static Ref<VertexBuffer> Create(uint32_t size);
  static Ref<VertexBuffer> Create(float *vertices, uint32_t size);
};

class IndexBuffer {
 public:
  virtual ~IndexBuffer() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual uint32_t GetCount() const = 0;

  static Ref<IndexBuffer> Create(uint32_t *indices, uint32_t count);
};

}  // namespace Develle
#endif  // BUFFER_HPP_
