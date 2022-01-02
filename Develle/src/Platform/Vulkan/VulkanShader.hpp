#pragma once
#ifndef VULKANSHADER_HPP_
#define VULKANSHADER_HPP_

#include <Develle/Renderer/Shader.hpp>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan.hpp>

namespace Develle {

class VulkanShader : public Shader {
 public:
  VulkanShader(const std::string &filepath);
  VulkanShader(const std::string &name, const std::string &vertShaderSource,
               const std::string &fragShaderSource);

  void SetInt(const std::string &name, int value) override;
  void SetIntArray(const std::string &name, int *values, uint32_t count) override;
  void SetFloat(const std::string &name, float value) override;
  void SetFloat2(const std::string &name, const glm::vec2 &value) override;
  void SetFloat3(const std::string &name, const glm::vec3 &value) override;
  void SetFloat4(const std::string &name, const glm::vec4 &value) override;
  void SetMat4(const std::string &name, const glm::mat4 &value) override;

  const std::string &GetName() const override { return name; }

  void UploadUniformInt(const std::string &name, int value);
  void UploadUniformIntArray(const std::string &name, int *values, uint32_t count);

  void UploadUniformFloat(const std::string &name, float value);
  void UploadUniformFloat2(const std::string &name, const glm::vec2 &value);
  void UploadUniformFloat3(const std::string &name, const glm::vec3 &value);
  void UploadUniformFloat4(const std::string &name, const glm::vec4 &value);

  void UploadUniformMat3(const std::string &name, const glm::mat3 &matrix);
  void UploadUniformMat4(const std::string &name, const glm::mat4 &matrix);

  void Bind() const override {}
  void Unbind() const override {}

  void *GetVertexModule() override { return &shaderModules[vk::ShaderStageFlagBits::eVertex]; }
  void *GetFragmentModule() override { return &shaderModules[vk::ShaderStageFlagBits::eFragment]; }

 private:
  std::string ReadFile(const std::string &filepath);
  std::unordered_map<vk::ShaderStageFlagBits, std::string> PreProcess(const std::string &source);

  void CompileOrGetVulkanBinaries(
      const std::unordered_map<vk::ShaderStageFlagBits, std::string> &shaderSources);

  void CreateShaderModules();

  std::string name;
  std::string filepath;
  std::unordered_map<vk::ShaderStageFlagBits, std::vector<uint32_t>> vulkanSPIRV;
  std::unordered_map<vk::ShaderStageFlagBits, vk::ShaderModule> shaderModules;
};

}  // namespace Develle
#endif  // VULKANSHADER_HPP_
