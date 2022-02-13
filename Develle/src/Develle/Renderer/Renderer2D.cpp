#include <Develle/Renderer/RenderCommand.hpp>
#include <Develle/Renderer/Renderer2D.hpp>
#include <Develle/Renderer/Shader.hpp>
#include <Develle/Renderer/UniformBuffer.hpp>
#include <Develle/Renderer/VertexArray.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
  // The number of actual vertices in a quad with 2 triangles
  static const uint32_t VerticesPerQuad = 6;
  // The number of vertices in a quad
  static const uint32_t QuadVertexCount = 4;
  constexpr static const std::array<glm::vec2, 4> TextureCoords = {
      glm::vec2{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

  constexpr static const std::array<glm::vec4, 4> QuadVertexPositions = {
      glm::vec4{-0.5f, -0.5f, 0.0f, 1.0f},  // NOLINT
      {0.5f, -0.5f, 0.0f, 1.0f},            // NOLINT
      {0.5f, 0.5f, 0.0f, 1.0f},             // NOLINT
      {-0.5f, 0.5f, 0.0f, 1.0f}};           // NOLINT

  Ref<VertexArray> QuadVertexArray;
  Ref<VertexBuffer> QuadVertexBuffer;
  Ref<Shader> TextureShader;
  Ref<Texture2D> WhiteTexture;

  uint32_t QuadIndexCount = 0;
  std::array<QuadVertex, MaxVertices> QuadVertexBufferBase{};
  std::array<QuadVertex, MaxVertices>::iterator QuadVertexBufferPtr = QuadVertexBufferBase.begin();

  std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
  uint32_t TextureSlotIndex = 1;  // 0 = white texture

  Renderer2D::Statistics Stats;

  struct CameraData {
    glm::mat4 ViewProjection;
  };
  CameraData CameraBuffer{};
  Ref<UniformBuffer> CameraUniformBuffer;
};

static Renderer2DData data;  // NOLINT

void Renderer2D::Init() {
  DV_PROFILE_FUNCTION();  // NOLINT

  data.QuadVertexArray = VertexArray::Create();

  data.QuadVertexBuffer = VertexBuffer::Create(data.MaxVertices * sizeof(QuadVertex));
  data.QuadVertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Position"},
                                    {ShaderDataType::Float4, "a_Color"},
                                    {ShaderDataType::Float2, "a_TexCoord"},
                                    {ShaderDataType::Float, "a_TexIndex"},
                                    {ShaderDataType::Float, "a_TilingFactor"},
                                    {ShaderDataType::Int, "a_EntityID"}});
  data.QuadVertexArray->AddVertexBuffer(data.QuadVertexBuffer);

  std::array<uint32_t, Renderer2DData::MaxIndices> quadIndices{};
  uint32_t offset = 0;
  for (size_t i = 0; i < quadIndices.size(); i += Renderer2DData::VerticesPerQuad) {
    quadIndices.at(i + 0) = offset + 0;
    quadIndices.at(i + 1) = offset + 1;
    quadIndices.at(i + 2) = offset + 2;

    quadIndices.at(i + 3) = offset + 2;
    quadIndices.at(i + 4) = offset + 3;
    quadIndices.at(i + 5) = offset + 0;  // NOLINT

    offset += 4;
  }

  Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices.data(), data.MaxIndices);
  data.QuadVertexArray->SetIndexBuffer(quadIB);

  data.WhiteTexture = Texture2D::Create(1, 1);
  uint32_t whiteTextureData = 0xFFFFFFFF;  // NOLINT
  data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

  // int32_t samplers[data.MaxTextureSlots];
  // for (uint32_t i = 0; i < data.MaxTextureSlots; i++) samplers[i] = i;

  data.TextureShader = Shader::Create("shaders/Texture.glsl");

  // Set first texture slot 0
  data.TextureSlots[0] = data.WhiteTexture;

  data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
}

void Renderer2D::Shutdown() {
  DV_PROFILE_FUNCTION();  // NOLINT
}

void Renderer2D::BeginScene(const EditorCamera &camera) {
  DV_PROFILE_FUNCTION();  // NOLINT

  data.CameraBuffer.ViewProjection = camera.GetViewProjection();
  data.CameraUniformBuffer->SetData(&data.CameraBuffer, sizeof(Renderer2DData::CameraData));

  StartBatch();
}

void Renderer2D::BeginScene(const OrthographicCamera &camera) {
  DV_PROFILE_FUNCTION();  // NOLINT

  data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
  data.CameraUniformBuffer->SetData(&data.CameraBuffer, sizeof(Renderer2DData::CameraData));

  StartBatch();
}

void Renderer2D::EndScene() {
  DV_PROFILE_FUNCTION();  // NOLINT
  Flush();
}

void Renderer2D::Flush() {
  DV_PROFILE_FUNCTION();                 // NOLINT
  if (data.QuadIndexCount == 0) return;  // Nothing to draw

  uint32_t vertexBufferSize = data.QuadVertexBufferPtr - data.QuadVertexBufferBase.begin();
  uint32_t dataSize = (uint32_t)(sizeof(QuadVertex) * (vertexBufferSize));
  data.QuadVertexBuffer->SetData(data.QuadVertexBufferBase.data(), dataSize);

  // Bind Textures
  for (uint32_t i = 0; i < data.TextureSlotIndex; i++) data.TextureSlots.at(i)->Bind(i);

  data.TextureShader->Bind();
  RenderCommand::DrawIndexed(data.QuadVertexArray, data.QuadIndexCount);
  data.Stats.DrawCalls++;
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
                          const glm::vec4 &color) {
  DrawQuad({position.x, position.y, 0.0f}, size, color);  // NOLINT
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size,
                          const glm::vec4 &color) {
  DV_PROFILE_FUNCTION();  // NOLINT
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});  // NOLINT
  DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
                          const Ref<Texture2D> &texture, float tilingFactor,
                          const glm::vec4 &tintColor) {
  DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);  // NOLINT
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size,
                          const Ref<Texture2D> &texture, float tilingFactor,
                          const glm::vec4 &tintColor) {
  DV_PROFILE_FUNCTION();  // NOLINT
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});  // NOLINT
  DrawQuad(transform, texture, tilingFactor, tintColor);
}

void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                                 const glm::vec4 &color) {
  DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);  // NOLINT
}

void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                                 const glm::vec4 &color) {
  DV_PROFILE_FUNCTION();  // NOLINT
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});  // NOLINT
  DrawQuad(transform, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                                 const Ref<Texture2D> &texture, float tilingFactor,
                                 const glm::vec4 &tintColor) {
  DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor,  // NOLINT
                  tintColor);                                                             // NOLINT
}

void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                                 const Ref<Texture2D> &texture, float tilingFactor,
                                 const glm::vec4 &tintColor) {
  DV_PROFILE_FUNCTION();  // NOLINT
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});  // NOLINT
  DrawQuad(transform, texture, tilingFactor, tintColor);
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, SpriteRendererComponent &src, int entityID) {
  if (src.Texture)
    DrawQuad(transform, src.Texture, src.TilingFactor, src.Color, entityID);
  else
    DrawQuad(transform, src.Color, entityID);
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const glm::vec4 &color, int entityID) {
  DV_PROFILE_FUNCTION();  // NOLINT

  const float textureIndex = 0.0f;  // White Texture
  const float tilingFactor = 1.0f;

  if (data.QuadIndexCount >= Renderer2DData::MaxIndices) NextBatch();

  for (size_t i = 0; i < Renderer2DData::QuadVertexCount; i++) {
    data.QuadVertexBufferPtr->Position = transform * data.QuadVertexPositions.at(i);
    data.QuadVertexBufferPtr->Color = color;
    data.QuadVertexBufferPtr->TexCoord = Renderer2DData::TextureCoords.at(i);
    data.QuadVertexBufferPtr->TexIndex = textureIndex;
    data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    data.QuadVertexBufferPtr->EntityID = entityID;
    data.QuadVertexBufferPtr++;
  }

  data.QuadIndexCount += Renderer2DData::VerticesPerQuad;

  data.Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const Ref<Texture2D> &texture,
                          float tilingFactor, const glm::vec4 &tintColor, int entityID) {
  DV_PROFILE_FUNCTION();  // NOLINT

  if (data.QuadIndexCount >= Renderer2DData::MaxIndices) NextBatch();

  float textureIndex = 0.0f;
  for (uint32_t i = 1; i < data.TextureSlotIndex; i++) {
    if (*data.TextureSlots.at(i) == *texture) {
      textureIndex = (float)i;
      break;
    }
  }

  if (textureIndex == 0.0f) {
    if (data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) NextBatch();

    textureIndex = (float)data.TextureSlotIndex;
    data.TextureSlots.at(data.TextureSlotIndex) = texture;
    data.TextureSlotIndex++;
  }

  for (size_t i = 0; i < Renderer2DData::QuadVertexCount; i++) {
    data.QuadVertexBufferPtr->Position = transform * Renderer2DData::QuadVertexPositions.at(i);
    data.QuadVertexBufferPtr->Color = tintColor;
    data.QuadVertexBufferPtr->TexCoord = Renderer2DData::TextureCoords.at(i);
    data.QuadVertexBufferPtr->TexIndex = textureIndex;
    data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    data.QuadVertexBufferPtr->EntityID = entityID;
    data.QuadVertexBufferPtr++;
  }

  data.QuadIndexCount += Renderer2DData::VerticesPerQuad;

  data.Stats.QuadCount++;
}

void Renderer2D::ResetStats() { data.Stats = {}; }

Renderer2D::Statistics Renderer2D::GetStats() { return data.Stats; }

void Renderer2D::StartBatch() {
  data.QuadIndexCount = 0;
  data.QuadVertexBufferPtr = data.QuadVertexBufferBase.begin();

  data.TextureSlotIndex = 1;
}

void Renderer2D::NextBatch() {
  Flush();
  StartBatch();
}

}  // namespace Develle
