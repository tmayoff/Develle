#include <Develle/Renderer/Shader.hpp>

#include <Platform/OpenGL/OpenGLShader.hpp>

namespace Develle {

Ref<Shader> Shader::Create(std::string filepath) {
  auto p = std::filesystem::path(filepath);
  if (!p.is_absolute())
    filepath = std::string(ASSETS_ROOT) + std::string("/") + filepath;

  // TODO(tyler) Add multiple APIs
  return CreateRef<OpenGLShader>(filepath);
}

Ref<Shader> Shader::Create(const std::string &name,
                           const std::string &vertexSource,
                           const std::string &fragmentSource) {
  // TODO(tyler) Add multiple APIs
  return CreateRef<OpenGLShader>(name, vertexSource, fragmentSource);
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

Ref<Shader> ShaderLibrary::Load(const std::string &name,
                                const std::string &filepath) {
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

} // namespace Develle
