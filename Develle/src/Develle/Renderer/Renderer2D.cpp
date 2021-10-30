#include <Develle/Renderer/Renderer2D.hpp>

#include <Develle/Renderer/RenderCommand.hpp>
#include <Develle/Renderer/Shader.hpp>
// #include <Develle/Renderer/UniformBuffer.hpp>
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

void Renderer2D::Init() {}

void Renderer2D::Shutdown() {}

void Renderer2D::BeginScene(const OrthographicCamera &camera) {}

void Renderer2D::EndScene() {}

void Renderer2D::Flush() {}

void Renderer2D::ResetStats() {}

Statistics Renderer2D::GetStats() {}

void Renderer2D::StartBatch() {}

void Renderer2D::NextBatch() {}

} // namespace Develle
