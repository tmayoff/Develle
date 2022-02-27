#include <Develle/Renderer/Shader.hpp>
#include <Platform/OpenGL/OpenGLShader.hpp>

namespace Develle {

auto Shader::Create(const std::string &filepath) -> Ref<Shader> {
  auto p = std::filesystem::path(filepath);
  std::string path;
  if (!p.is_absolute()) {
    path = std::string(ASSETS_ROOT) + std::string("/") + filepath;
  }

  // TODO(tyler) Add multiple APIs
  return CreateRef<OpenGLShader>(path);
}

auto Shader::Create(const std::string &name, const std::string &vertexSource,
                    const std::string &fragmentSource) -> Ref<Shader> {
  // TODO(tyler) Add multiple APIs
  return CreateRef<OpenGLShader>(name, vertexSource, fragmentSource);
}

void ShaderLibrary::Add(const std::string &name, const Ref<Shader> &shader) {
  DV_CORE_ASSERT(!Exists(name), "Shader already exists!");
  shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader> &shader) {
  const auto &name = shader->GetName();
  Add(name, shader);
}

auto ShaderLibrary::Load(const std::string &filepath) -> Ref<Shader> {
  auto shader = Shader::Create(filepath);
  Add(shader);
  return shader;
}

auto ShaderLibrary::Load(const std::string &name, const std::string &filepath) -> Ref<Shader> {
  auto shader = Shader::Create(filepath);
  Add(name, shader);
  return shader;
}

auto ShaderLibrary::Get(const std::string &name) -> Ref<Shader> {
  DV_CORE_ASSERT(Exists(name), "Shader not found");
  return shaders[name];
}

auto ShaderLibrary::Exists(const std::string &name) const -> bool {
  return shaders.find(name) != shaders.end();
}

}  // namespace Develle
