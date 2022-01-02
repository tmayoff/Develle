#include "OpenGLShader.hpp"

#include <glad/glad.h>

#include <Develle/Core/Timer.hpp>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

namespace Develle {

namespace Utils {

static GLenum OpenGLShaderTypeFromString(const std::string &type) {
  if (type == "vertex")
    return GL_VERTEX_SHADER;
  else if (type == "fragment" || type == "pixel")
    return GL_FRAGMENT_SHADER;

  DV_CORE_ASSERT(false, "Unknown shader type");
  return 0;
}

static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
  switch (stage) {
    case GL_VERTEX_SHADER:
      return shaderc_glsl_vertex_shader;
    case GL_FRAGMENT_SHADER:
      return shaderc_glsl_fragment_shader;
    default:
      DV_CORE_ASSERT(false, "Unknown shader stage");
      return (shaderc_shader_kind)0;
  }
}

static const char *GLShaderStageToString(GLenum stage) {
  switch (stage) {
    case GL_VERTEX_SHADER:
      return "GL_VERTEX_SHADER";
    case GL_FRAGMENT_SHADER:
      return "GL_FRAGMENT_SHADER";
    default:
      DV_CORE_ASSERT(false, "Unknown shader stage");
      return nullptr;
  }
}

static const char *GLShaderStageCachedOpenGLFileExtension(uint32_t stage) {
  switch (stage) {
    case GL_VERTEX_SHADER:
      return ".cached_opengl.vert";
    case GL_FRAGMENT_SHADER:
      return ".cached_opengl.frag";
    default:
      DV_CORE_ASSERT(false, "Unknown shader stage");
      return "";
  }
}

static const char *GLShaderStageCachedVulkanFileExtension(uint32_t stage) {
  switch (stage) {
    case GL_VERTEX_SHADER:
      return ".cached_vulkan.vert";
    case GL_FRAGMENT_SHADER:
      return ".cached_vulkan.frag";
    default:
      DV_CORE_ASSERT(false, "Unknown shader stage");
      return "";
  }
}

}  // namespace Utils

OpenGLShader::OpenGLShader(const std::string &filepath) : filepath(filepath) {
  DV_PROFILE_FUNCTION();

  Utils::CreateCacheDirectoryIfNeeded("opengl");

  std::string source = ReadFile(filepath);
  auto shaderSources = PreProcess(source);

  {
    Timer timer;
    CompileOrGetVulkanBinaries(shaderSources);
    CompileOrGetOpenGLBinaries();
    CreateProgram();
    DV_CORE_WARN("Shader creation took {0}ms", timer.ElapsedMillis());
  }

  auto path = std::filesystem::path(filepath);
  name = path.filename();
}

OpenGLShader::OpenGLShader(const std::string &name, const std::string &vertexSource,
                           const std::string &fragmentSource)
    : name(name) {
  DV_PROFILE_FUNCTION();

  std::unordered_map<GLenum, std::string> sources;
  sources[GL_VERTEX_SHADER] = vertexSource;
  sources[GL_FRAGMENT_SHADER] = fragmentSource;

  CompileOrGetVulkanBinaries(sources);
  CompileOrGetOpenGLBinaries();
  CreateProgram();
}

OpenGLShader::~OpenGLShader() {
  DV_PROFILE_FUNCTION();
  glDeleteProgram(rendererID);
}

void OpenGLShader::Bind() const {
  DV_PROFILE_FUNCTION();
  glUseProgram(rendererID);
}

void OpenGLShader::Unbind() const {
  DV_PROFILE_FUNCTION();
  glUseProgram(0);
}

void OpenGLShader::SetInt(const std::string &name, int value) {
  DV_PROFILE_FUNCTION();
  UploadUniformInt(name, value);
}

void OpenGLShader::SetIntArray(const std::string &name, int *values, uint32_t count) {
  DV_PROFILE_FUNCTION();
  UploadUniformIntArray(name, values, count);
}

void OpenGLShader::SetFloat(const std::string &name, float value) {
  DV_PROFILE_FUNCTION();
  UploadUniformFloat(name, value);
}

void OpenGLShader::SetFloat2(const std::string &name, const glm::vec2 &value) {
  DV_PROFILE_FUNCTION();
  UploadUniformFloat2(name, value);
}

void OpenGLShader::SetFloat3(const std::string &name, const glm::vec3 &value) {
  DV_PROFILE_FUNCTION();
  UploadUniformFloat3(name, value);
}

void OpenGLShader::SetFloat4(const std::string &name, const glm::vec4 &value) {
  DV_PROFILE_FUNCTION();
  UploadUniformFloat4(name, value);
}

void OpenGLShader::SetMat4(const std::string &name, const glm::mat4 &value) {
  DV_PROFILE_FUNCTION();
  UploadUniformMat4(name, value);
}

void OpenGLShader::UploadUniformInt(const std::string &name, int value) {
  GLint location = glGetUniformLocation(rendererID, name.c_str());
  glUniform1i(location, value);
}

void OpenGLShader::UploadUniformIntArray(const std::string &name, int *values, uint32_t count) {
  GLint location = glGetUniformLocation(rendererID, name.c_str());
  glUniform1iv(location, count, values);
}

void OpenGLShader::UploadUniformFloat(const std::string &name, float value) {
  GLint location = glGetUniformLocation(rendererID, name.c_str());
  glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string &name, const glm::vec2 &value) {
  GLint location = glGetUniformLocation(rendererID, name.c_str());
  glUniform2f(location, value.x, value.y);
}

void OpenGLShader::UploadUniformFloat3(const std::string &name, const glm::vec3 &value) {
  GLint location = glGetUniformLocation(rendererID, name.c_str());
  glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::UploadUniformFloat4(const std::string &name, const glm::vec4 &value) {
  GLint location = glGetUniformLocation(rendererID, name.c_str());
  glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniformMat3(const std::string &name, const glm::mat3 &matrix) {
  GLint location = glGetUniformLocation(rendererID, name.c_str());
  glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string &name, const glm::mat4 &matrix) {
  GLint location = glGetUniformLocation(rendererID, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string OpenGLShader::ReadFile(const std::string &filepath) {
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

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string &source) {
  DV_PROFILE_FUNCTION();

  std::unordered_map<GLenum, std::string> shaderSources;
  const char *typeToken = "#type";
  size_t typeTokenLength = strlen(typeToken);
  size_t pos = source.find(typeToken, 0);  // Start of shader type declaration
  while (pos != std::string::npos) {
    size_t eol = source.find_first_of("\r\n", pos);  // End of shader type declaration
    DV_CORE_ASSERT(eol != std::string::npos, "Syntax error");
    size_t begin = pos + typeTokenLength + 1;  // Start of shader type name (after '#type' keyword)
    std::string type = source.substr(begin, eol - begin);
    DV_CORE_ASSERT(Utils::OpenGLShaderTypeFromString(type), "Invalid shader type specified");

    size_t nextLinePos = source.find_first_not_of(
        "\r\n", eol);  // Start of shader code after shader type declaration line
    DV_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
    pos = source.find(typeToken, nextLinePos);

    shaderSources[Utils::OpenGLShaderTypeFromString(type)] =
        (pos == std::string::npos) ? source.substr(nextLinePos)
                                   : source.substr(nextLinePos, pos - nextLinePos);
  }

  return shaderSources;
}

void OpenGLShader::CompileOrGetVulkanBinaries(
    const std::unordered_map<GLenum, std::string> &shaderSources) {
  shaderc::Compiler compiler;
  shaderc::CompileOptions options;
  options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
  const bool optimize = true;
  if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_performance);

  std::filesystem::path cacheDirectory = Utils::GetCacheDirectory("opengl");

  auto &shaderData = vulkanSPIRV;
  shaderData.clear();
  for (auto &&[stage, source] : shaderSources) {
    std::filesystem::path shaderFilepath = filepath;
    std::filesystem::path cachedPath =
        cacheDirectory /
        (shaderFilepath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

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
          source, Utils::GLShaderStageToShaderC(stage), filepath.c_str(), options);
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

  for (auto &&[stage, data] : shaderData) Reflect(stage, data);
}

void OpenGLShader::CompileOrGetOpenGLBinaries() {
  auto &shaderData = openGLSPIRV;

  shaderc::Compiler compiler;
  shaderc::CompileOptions options;
  options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
  const bool optimize = false;
  if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_performance);

  std::filesystem::path cacheDirectory = Utils::GetCacheDirectory("opengl");

  shaderData.clear();
  openGLSourceCode.clear();
  for (auto &&[stage, spirv] : vulkanSPIRV) {
    std::filesystem::path shaderFilepath = filepath;
    std::filesystem::path cachedPath =
        cacheDirectory /
        (shaderFilepath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

    std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
    if (in.is_open()) {
      in.seekg(0, std::ios::end);
      auto size = in.tellg();
      in.seekg(0, std::ios::beg);

      auto &data = shaderData[stage];
      data.resize(size / sizeof(uint32_t));
      in.read((char *)data.data(), size);
    } else {
      spirv_cross::CompilerGLSL glslCompiler(spirv);
      openGLSourceCode[stage] = glslCompiler.compile();
      auto &source = openGLSourceCode[stage];

      shaderc::SpvCompilationResult module =
          compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), filepath.c_str());
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
}

void OpenGLShader::CreateProgram() {
  GLuint program = glCreateProgram();

  std::vector<GLuint> shaderIDs;
  for (auto &&[stage, spirv] : openGLSPIRV) {
    GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
    glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(),
                   spirv.size() * sizeof(uint32_t));
    glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
    glAttachShader(program, shaderID);
  }

  glLinkProgram(program);

  GLint isLinked;
  glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
    DV_CORE_ERROR("Shader linking failed ({0}):\n{1}", filepath, infoLog.data());

    glDeleteProgram(program);

    for (auto id : shaderIDs) glDeleteShader(id);
  }

  for (auto id : shaderIDs) {
    glDetachShader(program, id);
    glDeleteShader(id);
  }

  rendererID = program;
}

void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t> &shaderData) {
  spirv_cross::Compiler compiler(shaderData);
  spirv_cross::ShaderResources resources = compiler.get_shader_resources();

  DV_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), filepath);
  DV_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
  DV_CORE_TRACE("    {0} resources", resources.sampled_images.size());

  DV_CORE_TRACE("Uniform buffers:");
  for (const auto &resource : resources.uniform_buffers) {
    const auto &bufferType = compiler.get_type(resource.base_type_id);
    uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
    uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
    int memberCount = bufferType.member_types.size();

    DV_CORE_TRACE("  {0}", resource.name);
    DV_CORE_TRACE("    Size = {0}", bufferSize);
    DV_CORE_TRACE("    Binding = {0}", binding);
    DV_CORE_TRACE("    Members = {0}", memberCount);
  }
}
}  // namespace Develle
