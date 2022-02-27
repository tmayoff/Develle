#pragma once
#ifndef OPENGLSHADER_HPP_
#define OPENGLSHADER_HPP_

#include <Develle/Renderer/Shader.hpp>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

using GLenum = uint32_t;

namespace Develle {

class OpenGLShader : public Shader {
 public:
  explicit OpenGLShader(const std::string &filepath);
  OpenGLShader(const std::string &name, const std::string &vertexSource,
               const std::string &fragmentSource);
  ~OpenGLShader() override;

  void Bind() const override;
  void Unbind() const override;

  void SetInt(const std::string &name, int value) override;
  void SetIntArray(const std::string &name, int *values, uint32_t count) override;
  void SetFloat(const std::string &name, float value) override;
  void SetFloat2(const std::string &name, const glm::vec2 &value) override;
  void SetFloat3(const std::string &name, const glm::vec3 &value) override;
  void SetFloat4(const std::string &name, const glm::vec4 &value) override;
  void SetMat4(const std::string &name, const glm::mat4 &value) override;

  [[nodiscard]] auto GetName() const -> const std::string & override { return name; }

  void UploadUniformInt(const std::string &name, int value);
  void UploadUniformIntArray(const std::string &name, int *values, uint32_t count);

  void UploadUniformFloat(const std::string &name, float value);
  void UploadUniformFloat2(const std::string &name, const glm::vec2 &value);
  void UploadUniformFloat3(const std::string &name, const glm::vec3 &value);
  void UploadUniformFloat4(const std::string &name, const glm::vec4 &value);

  void UploadUniformMat3(const std::string &name, const glm::mat3 &matrix);
  void UploadUniformMat4(const std::string &name, const glm::mat4 &matrix);

 private:
  static auto ReadFile(const std::string &filepath) -> std::string;
  static auto PreProcess(const std::string &source) -> std::unordered_map<GLenum, std::string>;
  void Compile(const std::unordered_map<GLenum, std::string> &shaders);

  uint32_t rendererID = -1;

  std::string filepath;
  std::string name;
};

}  // namespace Develle
#endif  // OPENGLSHADER_HPP_
