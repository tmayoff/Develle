#pragma once
#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <filesystem>
#include <glm/glm.hpp>

namespace Develle {
namespace Utils {

std::string GetCacheDirectory(const std::string &api);
void CreateCacheDirectoryIfNeeded(const std::string &api);

}  // namespace Utils

class Shader {
 public:
  virtual ~Shader() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void SetInt(const std::string &name, int value) = 0;
  virtual void SetIntArray(const std::string &name, int *values, uint32_t count) = 0;
  virtual void SetFloat(const std::string &name, float value) = 0;
  virtual void SetFloat2(const std::string &name, const glm::vec2 &value) = 0;
  virtual void SetFloat3(const std::string &name, const glm::vec3 &value) = 0;
  virtual void SetFloat4(const std::string &name, const glm::vec4 &value) = 0;
  virtual void SetMat4(const std::string &name, const glm::mat4 &value) = 0;

  virtual const std::string &GetName() const = 0;

  virtual void *GetVertexModule() = 0;
  virtual void *GetFragmentModule() = 0;

  static Ref<Shader> Create(std::string filepath);
  static Ref<Shader> Create(const std::string &name, const std::string &vertexSource,
                            const std::string &fragmentSource);
};

class ShaderLibrary {
 public:
  void Add(const std::string &name, const Ref<Shader> &shader);
  void Add(const Ref<Shader> &shader);
  Ref<Shader> Load(const std::string &filepath);
  Ref<Shader> Load(const std::string &name, const std::string &filepath);

  Ref<Shader> Get(const std::string &name);

  bool Exists(const std::string &name) const;

 private:
  std::unordered_map<std::string, Ref<Shader>> shaders;
};

}  // namespace Develle

#endif  // SHADER_HPP_
