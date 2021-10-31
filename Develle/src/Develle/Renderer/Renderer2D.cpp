#include <Develle/Renderer/Renderer2D.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Develle/Renderer/RenderCommand.hpp>
#include <Develle/Renderer/Shader.hpp>
#include <Develle/Renderer/Texture.hpp>
#include <Develle/Renderer/UniformBuffer.hpp>
#include <Develle/Renderer/VertexArray.hpp>

namespace Develle {

struct QuadVertex {
  glm::vec3 Position;
  glm::vec4 Color;
  glm::vec2 TexCoord;
  float TexIndex;
  float TilingFactor;

  // Editor Only
  int entityID;
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
  uint32_t TextureSlotIndex = 1; // 0 = white texture

  glm::vec4 QuadVertexPositions[4];

  Renderer2D::Statistics Stats;

  struct CameraData {
    glm::mat4 ViewProjetion;
  };
  CameraData CameraBuffer;
  Ref<UniformBuffer> CameraUniformBuffer;
};

static Renderer2DData data;

void Renderer2D::Init() {
  DV_PROFILE_FUNCTION();

  data.QuadVertexArray = VertexArray::Create();

  data.QuadVertexBuffer =
      VertexBuffer::Create(data.MaxVertices * sizeof(QuadVertex));
  data.QuadVertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Position"},
                                    {ShaderDataType::Float4, "a_Color"},
                                    {ShaderDataType::Float2, "a_TexCoord"},
                                    {ShaderDataType::Float, "a_TexIndex"},
                                    {ShaderDataType::Float, "a_TilingFactor"},
                                    {ShaderDataType::Int, "a_EntityID"}});
  data.QuadVertexArray->AddVertexBuffer(data.QuadVertexBuffer);

  data.QuadVertexBufferBase = new QuadVertex[data.MaxVertices];

  uint32_t *quadIndices = new uint32_t[data.MaxIndices];

  uint32_t offset = 0;
  for (uint32_t i = 0; i < data.MaxIndices; i += 6) {
    quadIndices[i + 0] = offset + 0;
    quadIndices[i + 1] = offset + 1;
    quadIndices[i + 2] = offset + 2;

    quadIndices[i + 3] = offset + 2;
    quadIndices[i + 4] = offset + 3;
    quadIndices[i + 5] = offset + 0;

    offset += 4;
  }

  Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, data.MaxIndices);
  data.QuadVertexArray->SetIndexBuffer(quadIB);
  delete[] quadIndices;

  data.WhiteTexture = Texture2D::Create(1, 1);
  uint32_t whiteTextureData = 0xFFFFFFFF;
  data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

  int32_t samplers[data.MaxTextureSlots];
  for (uint32_t i = 0; i < data.MaxTextureSlots; i++)
    samplers[i] = i;

  data.TextureShader = Shader::Create("shaders/Texture.glsl");

  // Set first texture slot 0
  data.TextureSlots[0] = data.WhiteTexture;

  data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
  data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
  data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
  data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};

  data.CameraUniformBuffer =
      UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
}

void Renderer2D::Shutdown() {
  DV_PROFILE_FUNCTION();
  delete[] data.QuadVertexBufferBase;
}

void Renderer2D::BeginScene(const OrthographicCamera &camera) {
  DV_PROFILE_FUNCTION();

  data.CameraBuffer.ViewProjetion = camera.GetViewProjectionMatrix();
  data.CameraUniformBuffer->SetData(&data.CameraBuffer,
                                    sizeof(Renderer2DData::CameraData));

  StartBatch();
}

void Renderer2D::EndScene() {
  DV_PROFILE_FUNCTION();
  Flush();
}

void Renderer2D::Flush() {
  if (data.QuadIndexCount == 0)
    return; // Nothing to draw

  uint32_t dataSize = (uint32_t)((uint8_t *)data.QuadVertexBufferPtr -
                                 (uint8_t *)data.QuadVertexBufferBase);
  data.QuadVertexBuffer->SetData(data.QuadVertexBufferBase, dataSize);

  // Bind Textures
  for (uint32_t i = 0; i < data.TextureSlotIndex; i++)
    data.TextureSlots[i]->Bind();

  data.TextureShader->Bind();
  RenderCommand::DrawIndexed(data.QuadVertexArray, data.QuadIndexCount);
  data.Stats.DrawCalls++;
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
                          const glm::vec4 &color) {}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size,
                          const glm::vec4 &color) {
  DV_PROFILE_FUNCTION();
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
  DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
                          const Ref<Texture2D> &texture, float tilingFactor,
                          const glm::vec4 &tintColor) {}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size,
                          const Ref<Texture2D> &texture, float tilingFactor,
                          const glm::vec4 &tintColor) {}

void Renderer2D::DrawQuad(const glm::mat4 &transform,
                          SpriteRendererComponent &src, int entityID) {}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const glm::vec4 &color,
                          int entityID) {
  DV_PROFILE_FUNCTION();

  constexpr size_t quadVertexCount = 4;
  constexpr glm::vec2 textureCoords[] = {
      {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
  const float textureIndex = 0.0f; // White Texture
  const float tilingFactor = 1.0f;

  if (data.QuadIndexCount >= Renderer2DData::MaxIndices)
    NextBatch();

  for (size_t i = 0; i < quadVertexCount; i++) {
    data.QuadVertexBufferPtr->Position =
        transform * data.QuadVertexPositions[i];
    data.QuadVertexBufferPtr->Color = color;
    data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
    data.QuadVertexBufferPtr->TexIndex = textureIndex;
    data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    data.QuadVertexBufferPtr->entityID = entityID;
    data.QuadVertexBufferPtr++;
  }

  data.QuadIndexCount += 6;

  data.Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::mat4 &transform,
                          const Ref<Texture2D> &texture, float tilingFactor,
                          const glm::vec4 &tintColor, int entityID) {}

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

} // namespace Develle
