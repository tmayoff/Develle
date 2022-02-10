#include "Renderer.hpp"

#include "Pipeline.hpp"
#include "Renderer2D.hpp"

namespace Develle {

void Renderer::Init() {
  DV_PROFILE_FUNCTION();

  // Initialize Pipeline
  PipelineOptions options;
  options.bufferLayout = {
      {ShaderDataType::Float3, "a_Position"},    {ShaderDataType::Float4, "a_Color"},
      {ShaderDataType::Float2, "a_TexCoord"},    {ShaderDataType::Float, "a_TexIndex"},
      {ShaderDataType::Float, "a_TilingFactor"}, {ShaderDataType::Int, "a_EntityID"}};
  options.shader = Shader::Create("shaders/Texture.glsl");
  options.descriptorLayouts = {DescriptorLayout{0, DescriptorType::UNIFORM_BUFFER}};

  RenderCommand::Init(options);
  Renderer2D::Init();
}

void Renderer::Shutdown() { Renderer2D::Shutdown(); }

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
  RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene() {}

void Renderer::EndScene() {}

}  // namespace Develle
