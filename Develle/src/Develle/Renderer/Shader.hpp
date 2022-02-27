#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <Develle/Core/Core.hpp>
#include <glm/glm.hpp>
#include <string>

namespace Develle {

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

  [[nodiscard]] virtual auto GetName() const -> const std::string & = 0;

  static auto Create(const std::string &filepath) -> Ref<Shader>;
  static auto Create(const std::string &name, const std::string &vertexSource,
                     const std::string &fragmentSource) -> Ref<Shader>;
};

class ShaderLibrary {
 public:
  void Add(const std::string &name, const Ref<Shader> &shader);
  void Add(const Ref<Shader> &shader);
  auto Load(const std::string &filepath) -> Ref<Shader>;
  auto Load(const std::string &name, const std::string &filepath) -> Ref<Shader>;

  auto Get(const std::string &name) -> Ref<Shader>;

  auto Exists(const std::string &name) const -> bool;

 private:
  std::unordered_map<std::string, Ref<Shader>> shaders;
};

}  // namespace Develle
#endif  // SHADER_HPP_
