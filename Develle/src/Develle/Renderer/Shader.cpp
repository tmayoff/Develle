#include "Shader.hpp"

#include <Platform/OpenGL/OpenGLShader.hpp>
#include <Platform/Vulkan/VulkanShader.hpp>

#include "RendererAPI.hpp"

namespace Develle {

namespace Utils {

std::string GetCacheDirectory(const std::string &api) {
  return std::string(ASSETS_ROOT) + "/cache/shader/" + api;
}

void CreateCacheDirectoryIfNeeded(const std::string &api) {
  std::string cacheDirectory = GetCacheDirectory(api);
  if (!std::filesystem::exists(cacheDirectory)) std::filesystem::create_directories(cacheDirectory);
}

}  // namespace Utils

Ref<Shader> Shader::Create(std::string filepath) {
  auto p = std::filesystem::path(filepath);
  if (!p.is_absolute()) filepath = std::string(ASSETS_ROOT) + std::string("/") + filepath;

  switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLShader>(filepath);
    case RendererAPI::API::Vulkan:
      return CreateScope<VulkanShader>(filepath);
    default:
      return nullptr;
  }
}

Ref<Shader> Shader::Create(const std::string &name, const std::string &vertexSource,
                           const std::string &fragmentSource) {
  switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLShader>(name, vertexSource, fragmentSource);
    case RendererAPI::API::Vulkan:
      return CreateScope<VulkanShader>(name, vertexSource, fragmentSource);
    default:
      return nullptr;
  }
}

void ShaderLibrary::Add(const std::string &name, const Ref<Shader> &shader) {
  DV_CORE_ASSERT(!Exists(name), "Shader already exists!");
  shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader> &shader) {
  auto &name = shader->GetName();
  Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string &filepath) {
  auto shader = Shader::Create(filepath);
  Add(shader);
  return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string &name, const std::string &filepath) {
  auto shader = Shader::Create(filepath);
  Add(name, shader);
  return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string &name) {
  DV_CORE_ASSERT(Exists(name), "Shader not found");
  return shaders[name];
}

bool ShaderLibrary::Exists(const std::string &name) const {
  return shaders.find(name) != shaders.end();
}

}  // namespace Develle
