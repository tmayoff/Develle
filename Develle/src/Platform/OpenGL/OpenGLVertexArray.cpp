#include <Platform/OpenGL/OpenGLVertexArray.hpp>

#include <glad/glad.h>

namespace Develle {

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
  case ShaderDataType::Float:
    return GL_FLOAT;
  case ShaderDataType::Float2:
    return GL_FLOAT;
  case ShaderDataType::Float3:
    return GL_FLOAT;
  case ShaderDataType::Float4:
    return GL_FLOAT;
  case ShaderDataType::Mat3:
    return GL_FLOAT;
  case ShaderDataType::Mat4:
    return GL_FLOAT;
  case ShaderDataType::Int:
    return GL_INT;
  case ShaderDataType::Int2:
    return GL_INT;
  case ShaderDataType::Int3:
    return GL_INT;
  case ShaderDataType::Int4:
    return GL_INT;
  case ShaderDataType::Bool:
    return GL_BOOL;
  default:
    DV_CORE_ASSERT(false, "Unknown ShaderDataType");
    return 0;
  }
}

OpenGLVertexArray::OpenGLVertexArray() {
  DV_PROFILE_FUNCTION();

  glCreateVertexArrays(1, &rendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
  DV_PROFILE_FUNCTION();

  glDeleteVertexArrays(1, &rendererID);
}

void OpenGLVertexArray::Bind() const {
  DV_PROFILE_FUNCTION();

  glBindVertexArray(rendererID);
}

void OpenGLVertexArray::Unbind() const {
  DV_PROFILE_FUNCTION();

  glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) {
  DV_PROFILE_FUNCTION();

  DV_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(),
                 "VertexBuffer has no layout");

  glBindVertexArray(rendererID);
  vertexBuffer->Bind();

  const auto &layout = vertexBuffer->GetLayout();
  for (const auto &element : layout) {
    switch (element.Type) {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4: {
      glEnableVertexAttribArray(vertexBufferIndex);
      glVertexAttribPointer(vertexBufferIndex, element.GetComponentCount(),
                            ShaderDataTypeToOpenGLBaseType(element.Type),
                            element.Normalized ? GL_TRUE : GL_FALSE,
                            layout.GetStride(), (const void *)element.Offset);
      vertexBufferIndex++;
      break;
    }
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
    case ShaderDataType::Bool: {
      glEnableVertexAttribArray(vertexBufferIndex);
      glVertexAttribIPointer(vertexBufferIndex, element.GetComponentCount(),
                             ShaderDataTypeToOpenGLBaseType(element.Type),
                             layout.GetStride(), (const void *)element.Offset);
      vertexBufferIndex++;
      break;
    }
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4: {
      uint8_t count = element.GetComponentCount();
      for (uint8_t i = 0; i < count; i++) {
        glEnableVertexAttribArray(vertexBufferIndex);
        glVertexAttribPointer(
            vertexBufferIndex, count,
            ShaderDataTypeToOpenGLBaseType(element.Type),
            element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
            (const void *)(element.Offset + sizeof(float) * count * i));
        glVertexAttribDivisor(vertexBufferIndex, 1);
        vertexBufferIndex++;
      }
      break;
    }

    default:
      DV_CORE_ASSERT(false, "Unknown ShaderDataType");
    }
  }

  vertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) {
  DV_PROFILE_FUNCTION();

  glBindVertexArray(rendererID);
  indexBuffer->Bind();

  this->indexBuffer = indexBuffer;
}

} // namespace Develle
