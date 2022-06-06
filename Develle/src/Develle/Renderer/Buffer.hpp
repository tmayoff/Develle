#ifndef BUFFER_HPP_
#define BUFFER_HPP_

#include <Develle/Core/Core.hpp>
#include <cstdint>
#include <utility>

namespace Develle {

enum class ShaderDataType {
  None = 0,
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
  Bool
};

static auto ShaderDataTypeSize(ShaderDataType type) -> uint32_t {
  switch (type) {
    case ShaderDataType::Float:
      return 4;
    case ShaderDataType::Float2:
      return 4 * 2;
    case ShaderDataType::Float3:
      return 4 * 3;
    case ShaderDataType::Float4:
      return 4 * 4;
    case ShaderDataType::Mat3:
      return 4 * 3 * 3;
    case ShaderDataType::Mat4:
      return 4 * 3 * 4;
    case ShaderDataType::Int:
      return 4;
    case ShaderDataType::Int2:
      return 4 * 2;
    case ShaderDataType::Int3:
      return 4 * 3;
    case ShaderDataType::Int4:
      return 4 * 4;
    case ShaderDataType::Bool:
      return 1;
    default:
      DV_CORE_ASSERT(false, "Unknown ShaderDataType");
      return 0;
  }
}

struct BufferElement {
  std::string Name;
  ShaderDataType Type;
  uint32_t Size{};
  size_t Offset{};
  bool Normalized{};

  BufferElement() = default;
  BufferElement(ShaderDataType type, std::string name, bool normalized = false)
      : Name(std::move(name)), Type(type), Size(ShaderDataTypeSize(type)), Normalized(normalized) {}

  [[nodiscard]] auto GetComponentCount() const -> uint32_t {
    switch (Type) {
      case ShaderDataType::Float:
        return 1;
      case ShaderDataType::Float2:
        return 2;
      case ShaderDataType::Float3:
        return 3;
      case ShaderDataType::Float4:
        return 4;
      case ShaderDataType::Mat3:
        return 3;  // 3* float3
      case ShaderDataType::Mat4:
        return 4;  // 4* float4
      case ShaderDataType::Int:
        return 1;
      case ShaderDataType::Int2:
        return 2;
      case ShaderDataType::Int3:
        return 3;
      case ShaderDataType::Int4:
        return 4;
      case ShaderDataType::Bool:
        return 1;
      default:
        DV_CORE_ASSERT(false, "Unknown ShaderDataType");
        return 0;
    }
  }
};

class BufferLayout {
 public:
  BufferLayout() = default;
  BufferLayout(std::initializer_list<BufferElement> elements) : elements(elements) {
    CalculateOffsetsAndStride();
  }

  [[nodiscard]] auto GetStride() const -> uint32_t { return stride; }
  [[nodiscard]] auto GetElements() const -> const std::vector<BufferElement> & { return elements; }

  auto begin() -> std::vector<BufferElement>::iterator { return elements.begin(); }
  auto end() -> std::vector<BufferElement>::iterator { return elements.end(); }
  [[nodiscard]] auto begin() const -> std::vector<BufferElement>::const_iterator {
    return elements.begin();
  }
  [[nodiscard]] auto end() const -> std::vector<BufferElement>::const_iterator {
    return elements.end();
  }

 private:
  void CalculateOffsetsAndStride() {
    size_t offset = 0;
    stride = 0;
    for (auto &element : elements) {
      element.Offset = offset;
      offset += element.Size;
      stride += element.Size;
    }
  }

  std::vector<BufferElement> elements;
  uint32_t stride = 0;
};

class VertexBuffer {
 public:
  virtual ~VertexBuffer() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void SetData(const void *data, uint32_t size) = 0;

  [[nodiscard]] virtual auto GetLayout() const -> const BufferLayout & = 0;
  virtual void SetLayout(const BufferLayout &layout) = 0;

  static auto Create(uint32_t size) -> Ref<VertexBuffer>;
  static auto Create(float *vertices, uint32_t size) -> Ref<VertexBuffer>;
};

class IndexBuffer {
 public:
  virtual ~IndexBuffer() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  [[nodiscard]] virtual auto GetCount() const -> uint32_t = 0;

  static auto Create(uint32_t *indices, uint32_t count) -> Ref<IndexBuffer>;
};

}  // namespace Develle
#endif  // BUFFER_HPP_
