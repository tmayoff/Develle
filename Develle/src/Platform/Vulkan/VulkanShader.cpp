#include "VulkanShader.hpp"

#include <Develle/Core/Application.hpp>
#include <Develle/Utils/ShaderUtils.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <fstream>
#include <shaderc/shaderc.hpp>
#include <vulkan/vulkan.hpp>

namespace Develle {

VulkanShader::VulkanShader(const std::string &filepath) : filepath(filepath) {
  DV_PROFILE_FUNCTION();

  std::string source = ReadFile(filepath);
  auto shaderSources = PreProcess(source);

  {
    CompileOrGetVulkanBinaries(shaderSources);
    CreateProgram();
  }

  auto path = std::filesystem::path(filepath);
  name = path.filename();
}

VulkanShader::VulkanShader(const std::string &name, const std::string &vertexSource,
                           const std::string &fragmentSource)
    : name(name) {
  DV_PROFILE_FUNCTION();

  std::unordered_map<VkShaderStageFlagBits, std::string> sources;
  sources[VK_SHADER_STAGE_VERTEX_BIT] = vertexSource;
  sources[VK_SHADER_STAGE_FRAGMENT_BIT] = fragmentSource;

  CompileOrGetVulkanBinaries(sources);
  CreateProgram();
}

VulkanShader::~VulkanShader() {}

void VulkanShader::Bind() const {}

void VulkanShader::Unbind() const {}

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

std::unordered_map<VkShaderStageFlagBits, std::string> VulkanShader::PreProcess(
    const std::string &source) {
  DV_PROFILE_FUNCTION();

  std::unordered_map<VkShaderStageFlagBits, std::string> shaderSources;
  const char *typeToken = "#type";
  size_t typeTokenLength = strlen(typeToken);
  size_t pos = source.find(typeToken, 0);  // Start of shader type declaration
  while (pos != std::string::npos) {
    size_t eol = source.find_first_of("\r\n", pos);  // End of shader type declaration
    DV_CORE_ASSERT(eol != std::string::npos, "Syntax error");
    size_t begin = pos + typeTokenLength + 1;  // Start of shader type name (after '#type' keyword)
    std::string type = source.substr(begin, eol - begin);
    DV_CORE_ASSERT(Utils::VulkanShaderTypeFromString(type), "Invalid shader type specified");

    size_t nextLinePos = source.find_first_not_of(
        "\r\n", eol);  // Start of shader code after shader type declaration line
    DV_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
    pos = source.find(typeToken, nextLinePos);

    shaderSources[Utils::VulkanShaderTypeFromString(type)] =
        (pos == std::string::npos) ? source.substr(nextLinePos)
                                   : source.substr(nextLinePos, pos - nextLinePos);
  }

  return shaderSources;
}

void VulkanShader::CompileOrGetVulkanBinaries(
    const std::unordered_map<VkShaderStageFlagBits, std::string> &shaderSources) {
  shaderc::Compiler compiler;
  shaderc::CompileOptions options;
  options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
  const bool optimize = true;
  if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_performance);

  std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

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
      shaderc::SpvCompilationResult res = compiler.CompileGlslToSpv(
          source, Utils::VulkanShaderStageToShaderC(stage), filepath.c_str(), options);
      if (res.GetCompilationStatus() != shaderc_compilation_status_success) {
        DV_CORE_ERROR(res.GetErrorMessage());
        DV_CORE_ASSERT(false, "");
      }

      shaderData[stage] = std::vector<uint32_t>(res.cbegin(), res.cend());

      std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
      if (out.is_open()) {
        auto &data = shaderData[stage];
        out.write((char *)data.data(), data.size() * sizeof(uint32_t));
        out.flush();
        out.close();
      }
    }
  }

  for (auto &&[stage, data] : shaderData) Reflect(stage, data);
}

void VulkanShader::CreateProgram() {
  std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;
  for (auto &&[stage, data] : vulkanSPIRV) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = data.size();
    createInfo.pCode = data.data();
    VkShaderModule shaderModule;

    auto context =
        static_cast<VulkanContext *>(Application::Get().GetWindow().GetGraphicsContext());
    vkCreateShaderModule(context->GetLogicalDevice(), &createInfo, nullptr, &shaderModule);

    VkPipelineShaderStageCreateInfo stageInfo{};
    stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.stage = stage;
    stageInfo.module = shaderModule;
    stageInfo.pName = "main";

    shaderStageCreateInfos.push_back(stageInfo);
  }
}

void VulkanShader::Reflect(VkShaderStageFlagBits stage, const std::vector<uint32_t> &shaderData) {}
}  // namespace Develle
