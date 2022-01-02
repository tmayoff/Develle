#include <Develle/Renderer/RenderCommand.hpp>
#include <Develle/Renderer/Renderer2D.hpp>
#include <Develle/Renderer/Shader.hpp>
#include <Develle/Renderer/UniformBuffer.hpp>
#include <Develle/Renderer/VertexArray.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Pipeline.hpp"

namespace Develle {

struct QuadVertex {
  glm::vec3 Position;
  glm::vec4 Color;
  glm::vec2 TexCoord;
  float TexIndex;
  float TilingFactor;

  // Editor Only
  int EntityID;
};

struct Renderer2DData {
  static const uint32_t MaxQuads = 20000;
  static const uint32_t MaxVertices = MaxQuads * 4;
  static const uint32_t MaxIndices = MaxQuads * 6;
  static const uint32_t MaxTextureSlots = 32;

  Ref<VertexArray> QuadVertexArray;
  Ref<VertexBuffer> QuadVertexBuffer;
  Ref<Shader> TextureShader;
  Ref<Texture2D> WhiteTexture;

  uint32_t QuadIndexCount = 0;
  QuadVertex *QuadVertexBufferBase = nullptr;
  QuadVertex *QuadVertexBufferPtr = nullptr;

  std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
  uint32_t TextureSlotIndex = 1;  // 0 = white texture

  glm::vec4 QuadVertexPositions[4];

  Renderer2D::Statistics Stats;

  struct CameraData {
    glm::mat4 ViewProjection;
  };
  CameraData CameraBuffer;
  Ref<UniformBuffer> CameraUniformBuffer;
};

static Renderer2DData data;

void Renderer2D::Init() {
  DV_PROFILE_FUNCTION();

  // Initialize Pipeline
  PipelineOptions options;
  options.bufferLayout = {
      {ShaderDataType::Float3, "a_Position"},    {ShaderDataType::Float4, "a_Color"},
      {ShaderDataType::Float2, "a_TexCoord"},    {ShaderDataType::Float, "a_TexIndex"},
      {ShaderDataType::Float, "a_TilingFactor"}, {ShaderDataType::Int, "a_EntityID"}};

  auto pipeLine = Pipeline::Create(options);

  data.QuadVertexBuffer = VertexBuffer::Create(data.MaxVertices * sizeof(QuadVertex));

  data.QuadVertexBufferBase = new QuadVertex[data.MaxVertices];

  std::array<uint32_t, data.MaxIndices> quadIndices{};

  uint32_t offset = 0;
  const uint32_t indicesPerQuad = 6;
  for (uint32_t i = 0; i < data.MaxIndices; i += indicesPerQuad) {
    quadIndices.at(i + 0) = offset + 0;
    quadIndices.at(i + 1) = offset + 1;
    quadIndices.at(i + 2) = offset + 2;

    quadIndices.at(i + 3) = offset + 2;
    quadIndices.at(i + 4) = offset + 3;
    quadIndices.at(i + 5) = offset + 0;

    offset += 4;
  }

  Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices.data(), data.MaxIndices);
  data.QuadVertexArray->SetIndexBuffer(quadIB);

  data.WhiteTexture = Texture2D::Create(1, 1);
  const uint32_t WhiteTextureData = 0xFFFFFFFF;
  data.WhiteTexture->SetData((void *)&WhiteTextureData, sizeof(uint32_t));

  std::array<uint32_t, data.MaxTextureSlots> samplers{};
  for (uint32_t i = 0; i < data.MaxTextureSlots; i++) samplers.at(i) = i;

  data.TextureShader = Shader::Create("shaders/Texture.glsl");

  // Set first texture slot 0
  data.TextureSlots[0] = data.WhiteTexture;

  data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
  data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
  data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
  data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};

  data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
}

void Renderer2D::Shutdown() {
  DV_PROFILE_FUNCTION();
  delete[] data.QuadVertexBufferBase;
}

void Renderer2D::BeginScene(const EditorCamera &camera) {
  DV_PROFILE_FUNCTION();

  data.CameraBuffer.ViewProjection = camera.GetViewProjection();
  data.CameraUniformBuffer->SetData(&data.CameraBuffer, sizeof(Renderer2DData::CameraData));

  StartBatch();
}

void Renderer2D::BeginScene(const OrthographicCamera &camera) {
  DV_PROFILE_FUNCTION();

  data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
  data.CameraUniformBuffer->SetData(&data.CameraBuffer, sizeof(Renderer2DData::CameraData));

  StartBatch();
}

void Renderer2D::EndScene() {
  DV_PROFILE_FUNCTION();
  Flush();
}

void Renderer2D::Flush() {
  if (data.QuadIndexCount == 0) return;  // Nothing to draw

  uint32_t dataSize =
      (uint32_t)((uint8_t *)data.QuadVertexBufferPtr - (uint8_t *)data.QuadVertexBufferBase);
  data.QuadVertexBuffer->SetData(data.QuadVertexBufferBase, dataSize);

  // Bind Textures
  for (uint32_t i = 0; i < data.TextureSlotIndex; i++) data.TextureSlots[i]->Bind(i);

  data.TextureShader->Bind();
  RenderCommand::DrawIndexed(data.QuadVertexArray, data.QuadIndexCount);
  data.Stats.DrawCalls++;
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
                          const glm::vec4 &color) {
  DrawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size,
                          const glm::vec4 &color) {
  DV_PROFILE_FUNCTION();
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
  DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
                          const Ref<Texture2D> &texture, float tilingFactor,
                          const glm::vec4 &tintColor) {
  DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size,
                          const Ref<Texture2D> &texture, float tilingFactor,
                          const glm::vec4 &tintColor) {
  DV_PROFILE_FUNCTION();
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
  DrawQuad(transform, texture, tilingFactor, tintColor);
}

void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                                 const glm::vec4 &color) {
  DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                                 const glm::vec4 &color) {
  DV_PROFILE_FUNCTION();
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
  DrawQuad(transform, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                                 const Ref<Texture2D> &texture, float tilingFactor,
                                 const glm::vec4 &tintColor) {
  DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor, tintColor);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                                 const Ref<Texture2D> &texture, float tilingFactor,
                                 const glm::vec4 &tintColor) {
  DV_PROFILE_FUNCTION();
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
  DrawQuad(transform, texture, tilingFactor, tintColor);
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, SpriteRendererComponent &src, int entityID) {
  if (src.Texture)
    DrawQuad(transform, src.Texture, src.TilingFactor, src.Color, entityID);
  else
    DrawQuad(transform, src.Color, entityID);
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const glm::vec4 &color, int entityID) {
  DV_PROFILE_FUNCTION();

  constexpr size_t quadVertexCount = 4;
  constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
  const float textureIndex = 0.0f;  // White Texture
  const float tilingFactor = 1.0f;

  if (data.QuadIndexCount >= Renderer2DData::MaxIndices) NextBatch();

  for (size_t i = 0; i < quadVertexCount; i++) {
    data.QuadVertexBufferPtr->Position = transform * data.QuadVertexPositions[i];
    data.QuadVertexBufferPtr->Color = color;
    data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
    data.QuadVertexBufferPtr->TexIndex = textureIndex;
    data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    data.QuadVertexBufferPtr->EntityID = entityID;
    data.QuadVertexBufferPtr++;
  }

  data.QuadIndexCount += 6;

  data.Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const Ref<Texture2D> &texture,
                          float tilingFactor, const glm::vec4 &tintColor, int entityID) {
  DV_PROFILE_FUNCTION();

  constexpr size_t quadVertexCount = 4;
  constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

  if (data.QuadIndexCount >= Renderer2DData::MaxIndices) NextBatch();

  float textureIndex = 0.0f;
  for (uint32_t i = 1; i < data.TextureSlotIndex; i++) {
    if (*data.TextureSlots[i] == *texture) {
      textureIndex = (float)i;
      break;
    }
  }

  if (textureIndex == 0.0f) {
    if (data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) NextBatch();

    textureIndex = (float)data.TextureSlotIndex;
    data.TextureSlots[data.TextureSlotIndex] = texture;
    data.TextureSlotIndex++;
  }

  for (size_t i = 0; i < quadVertexCount; i++) {
    data.QuadVertexBufferPtr->Position = transform * data.QuadVertexPositions[i];
    data.QuadVertexBufferPtr->Color = tintColor;
    data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
    data.QuadVertexBufferPtr->TexIndex = textureIndex;
    data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    data.QuadVertexBufferPtr->EntityID = entityID;
    data.QuadVertexBufferPtr++;
  }

  data.QuadIndexCount += 6;

  data.Stats.QuadCount++;
}

void Renderer2D::ResetStats() { data.Stats = {}; }

Renderer2D::Statistics Renderer2D::GetStats() { return data.Stats; }

void Renderer2D::StartBatch() {
  data.QuadIndexCount = 0;
  data.QuadVertexBufferPtr = data.QuadVertexBufferBase;

  data.TextureSlotIndex = 1;
}

void Renderer2D::NextBatch() {
  Flush();
  StartBatch();
}

}  // namespace Develle
