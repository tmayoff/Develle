#include "VulkanShader.hpp"

#include <fstream>
#include <shaderc/shaderc.hpp>

#include "VulkanContext.hpp"

namespace Develle {

namespace Utils {

static shaderc_shader_kind VulkanShaderStageToShaderC(vk::ShaderStageFlagBits stage) {
  switch (stage) {
    case vk::ShaderStageFlagBits::eVertex:
      return shaderc_glsl_vertex_shader;
    case vk::ShaderStageFlagBits::eFragment:
      return shaderc_glsl_fragment_shader;
    default:
      DV_CORE_ASSERT(false, "Unknown shader stage");
      return (shaderc_shader_kind)0;
  }
}

vk::ShaderStageFlagBits VulkanShaderStageFromString(const std::string &stage) {
  if (stage == "vertex")
    return vk::ShaderStageFlagBits::eVertex;
  else if (stage == "fragment" || stage == "pixel")
    return vk::ShaderStageFlagBits::eFragment;
}

static const char *VulkanShaderStageCachedVulkanFileExtension(vk::ShaderStageFlagBits stage) {
  switch (stage) {
    case vk::ShaderStageFlagBits::eVertex:
      return ".cached_vulkan.vert";
    case vk::ShaderStageFlagBits::eFragment:
      return ".cached_vulkan.frag";
    default:
      DV_CORE_ASSERT(false, "Unknown shader stage");
      return "";
  }
}

}  // namespace Utils

VulkanShader::VulkanShader(const std::string &filepath) : filepath(filepath) {
  DV_PROFILE_FUNCTION();

  Utils::CreateCacheDirectoryIfNeeded("vulkan");

  std::string source = ReadFile(filepath);
  auto shaderSources = PreProcess(source);

  CompileOrGetVulkanBinaries(shaderSources);
  CreateShaderModules();

  auto path = std::filesystem::path(filepath);
  name = path.filename();
}

VulkanShader::VulkanShader(const std::string &name, const std::string &vertShaderSource,
                           const std::string &fragShaderSource) {}

void VulkanShader::SetInt(const std::string &name, int value) {}

void VulkanShader::SetIntArray(const std::string &name, int *values, uint32_t count) {}

void VulkanShader::SetFloat(const std::string &name, float value) {}

void VulkanShader::SetFloat2(const std::string &name, const glm::vec2 &value) {}

void VulkanShader::SetFloat3(const std::string &name, const glm::vec3 &value) {}

void VulkanShader::SetFloat4(const std::string &name, const glm::vec4 &value) {}

void VulkanShader::SetMat4(const std::string &name, const glm::mat4 &value) {}

void VulkanShader::UploadUniformInt(const std::string &name, int value) {}

void VulkanShader::UploadUniformIntArray(const std::string &name, int *values, uint32_t count) {}

void VulkanShader::UploadUniformFloat(const std::string &name, float value) {}

void VulkanShader::UploadUniformFloat2(const std::string &name, const glm::vec2 &value) {}

void VulkanShader::UploadUniformFloat3(const std::string &name, const glm::vec3 &value) {}

void VulkanShader::UploadUniformFloat4(const std::string &name, const glm::vec4 &value) {}

void VulkanShader::UploadUniformMat3(const std::string &name, const glm::mat3 &matrix) {}

void VulkanShader::UploadUniformMat4(const std::string &name, const glm::mat4 &matrix) {}

std::string VulkanShader::ReadFile(const std::string &filepath) {
  DV_PROFILE_FUNCTION();

  std::string result;
  std::ifstream in(filepath, std::ios::in | std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    if (size != -1) {
      result.resize(size);
      in.seekg(0, std::ios::beg);
      in.read(&result[0], size);
    } else {
      DV_CORE_ERROR("Could not read from file '{0}'", filepath);
    }
  } else {
    DV_CORE_ERROR("Could not read file '{0}'", filepath);
  }

  return result;
}

std::unordered_map<vk::ShaderStageFlagBits, std::string> VulkanShader::PreProcess(
    const std::string &source) {
  DV_PROFILE_FUNCTION();

  std::unordered_map<vk::ShaderStageFlagBits, std::string> shaderSources;
  const char *typeToken = "#type";
  size_t typeTokenLength = strlen(typeToken);
  size_t pos = source.find(typeToken, 0);  // Start of shader type declaration
  while (pos != std::string::npos) {
    size_t eol = source.find_first_of("\r\n", pos);  // End of shader type declaration
    DV_CORE_ASSERT(eol != std::string::npos, "Syntax error");
    size_t begin = pos + typeTokenLength + 1;  // Start of shader type name (after '#type' keyword)
    std::string type = source.substr(begin, eol - begin);
    // DV_CORE_ASSERT(Utils::VulkanShaderStageFromString(type), "Invalid shader type specified");

    size_t nextLinePos = source.find_first_not_of(
        "\r\n", eol);  // Start of shader code after shader type declaration line
    DV_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
    pos = source.find(typeToken, nextLinePos);

    shaderSources[Utils::VulkanShaderStageFromString(type)] =
        (pos == std::string::npos) ? source.substr(nextLinePos)
                                   : source.substr(nextLinePos, pos - nextLinePos);
  }

  return shaderSources;
}

void VulkanShader::CompileOrGetVulkanBinaries(
    const std::unordered_map<vk::ShaderStageFlagBits, std::string> &shaderSources) {
  shaderc::Compiler compiler;
  shaderc::CompileOptions options;
  options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
  const bool optimize = true;
  if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_performance);

  std::filesystem::path cacheDirectory = Utils::GetCacheDirectory("vulkan");

  auto &shaderData = vulkanSPIRV;
  shaderData.clear();
  for (auto &&[stage, source] : shaderSources) {
    std::filesystem::path shaderFilepath = filepath;
    std::filesystem::path cachedPath =
        cacheDirectory / (shaderFilepath.filename().string() +
                          Utils::VulkanShaderStageCachedVulkanFileExtension(stage));

    std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
    if (in.is_open()) {
      in.seekg(0, std::ios::end);
      auto size = in.tellg();
      in.seekg(0, std::ios::beg);

      auto &data = shaderData[stage];
      data.resize(size / sizeof(uint32_t));
      in.read((char *)data.data(), size);
    } else {
      shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(
          source, Utils::VulkanShaderStageToShaderC(stage), filepath.c_str(), options);
      if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
        DV_CORE_ERROR(module.GetErrorMessage());
        DV_CORE_ASSERT(false);
      }

      shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

      std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
      if (out.is_open()) {
        auto &data = shaderData[stage];
        out.write((char *)data.data(), data.size() * sizeof(uint32_t));
        out.flush();
        out.close();
      }
    }
  }

  //   for (auto &&[stage, data] : shaderData) Reflect(stage, data);
}

void VulkanShader::CreateShaderModules() {
  auto &vulkan = GetCurrentVulkanContext();

  vk::ShaderModuleCreateInfo vertShaderInfo;
  vertShaderInfo.setCode(vulkanSPIRV[vk::ShaderStageFlagBits::eVertex]);
  shaderModules[vk::ShaderStageFlagBits::eVertex] =
      vulkan.GetDevice().createShaderModule(vertShaderInfo);

  vk::ShaderModuleCreateInfo fragShaderInfo;
  fragShaderInfo.setCode(vulkanSPIRV[vk::ShaderStageFlagBits::eFragment]);
  shaderModules[vk::ShaderStageFlagBits::eFragment] =
      vulkan.GetDevice().createShaderModule(fragShaderInfo);
}

}  // namespace Develle
