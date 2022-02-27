#include "OpenGLShader.hpp"

#include <glad/glad.h>

#include <Develle/Core/Core.hpp>
#include <Develle/Debug/Instrumentor.hpp>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace Utils {

auto ShaderTypeFromString(const std::string &shaderType) -> GLenum {
  if (shaderType == "vertex") return GL_VERTEX_SHADER;
  if (shaderType == "fragment" || shaderType == "pixel") return GL_FRAGMENT_SHADER;

  DV_CORE_ASSERT(false, "Unknown shader type");
  return 0;
}

}  // namespace Utils

namespace Develle {

OpenGLShader::OpenGLShader(const std::string &filepath) : filepath(filepath) {
  auto shaders = PreProcess(ReadFile(filepath));
  Compile(shaders);
}

OpenGLShader::OpenGLShader(const std::string &name, const std::string &vertexSource,
                           const std::string &fragmentSource)
    : name(name) {
  Compile({{GL_VERTEX_SHADER, vertexSource}, {GL_FRAGMENT_SHADER, fragmentSource}});
}

OpenGLShader::~OpenGLShader() {
  DV_PROFILE_FUNCTION();  // NOLINT
  glDeleteProgram(rendererID);
}

void OpenGLShader::Bind() const {
  DV_PROFILE_FUNCTION();  // NOLINT
  glUseProgram(rendererID);
}

void OpenGLShader::Unbind() const {
  DV_PROFILE_FUNCTION();  // NOLINT
  glUseProgram(0);
}

void OpenGLShader::SetInt(const std::string &name, int value) {
  DV_PROFILE_FUNCTION();  // NOLINT
  UploadUniformInt(name, value);
}

void OpenGLShader::SetIntArray(const std::string &name, int *values, uint32_t count) {
  DV_PROFILE_FUNCTION();  // NOLINT
  UploadUniformIntArray(name, values, count);
}

void OpenGLShader::SetFloat(const std::string &name, float value) {
  DV_PROFILE_FUNCTION();  // NOLINT
  UploadUniformFloat(name, value);
}

void OpenGLShader::SetFloat2(const std::string &name, const glm::vec2 &value) {
  DV_PROFILE_FUNCTION();  // NOLINT
  UploadUniformFloat2(name, value);
}

void OpenGLShader::SetFloat3(const std::string &name, const glm::vec3 &value) {
  DV_PROFILE_FUNCTION();  // NOLINT
  UploadUniformFloat3(name, value);
}

void OpenGLShader::SetFloat4(const std::string &name, const glm::vec4 &value) {
  DV_PROFILE_FUNCTION();  // NOLINT
  UploadUniformFloat4(name, value);
}

void OpenGLShader::SetMat4(const std::string &name, const glm::mat4 &value) {
  DV_PROFILE_FUNCTION();  // NOLINT
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

auto OpenGLShader::ReadFile(const std::string &filepath) -> std::string {
  DV_PROFILE_FUNCTION();  // NOLINT

  std::string result;
  std::ifstream in(filepath, std::ios::in | std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    auto size = in.tellg();
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

auto OpenGLShader::PreProcess(const std::string &source)
    -> std::unordered_map<GLenum, std::string> {
  DV_PROFILE_FUNCTION();  // NOLINT

  std::unordered_map<GLenum, std::string> shaderSources;
  const char *typeToken = "#type";
  size_t typeTokenLength = strlen(typeToken);
  size_t pos = source.find(typeToken, 0);  // Start of shader type declaration
  while (pos != std::string::npos) {
    size_t eol = source.find_first_of("\r\n", pos);  // End of shader type declaration
    DV_CORE_ASSERT(eol != std::string::npos, "Syntax error");
    size_t begin = pos + typeTokenLength + 1;  // Start of shader type name (after '#type' keywork)
    std::string type = source.substr(begin, eol - begin);
    DV_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

    size_t nextLinePos =
        source.find_first_not_of("\r\n",
                                 eol);  // Start of shader code after shadder type declaration line
    DV_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
    pos = source.find(typeToken, nextLinePos);

    shaderSources[Utils::ShaderTypeFromString(type)] =
        (pos == std::string::npos) ? source.substr(nextLinePos)
                                   : source.substr(nextLinePos, pos - nextLinePos);
  }

  return shaderSources;
}

void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> &shaders) {
  rendererID = glCreateProgram();

  for (const auto &shaderPair : shaders) {
    auto shader = glCreateShader(shaderPair.first);
    const auto *source = shaderPair.second.c_str();
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
      int maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
      std::string info;
      info.resize(maxLength);
      glGetShaderInfoLog(shader, maxLength, nullptr, info.data());
    }

    glAttachShader(rendererID, shader);
  }

  glLinkProgram(rendererID);
  GLint success = 0;
  glGetProgramiv(rendererID, GL_LINK_STATUS, &success);
  if (success != GL_TRUE) {
    int maxLength = 0;
    glGetShaderiv(rendererID, GL_INFO_LOG_LENGTH, &maxLength);
    std::string info;
    info.resize(maxLength);
    glGetShaderInfoLog(rendererID, maxLength, nullptr, info.data());
  }
}

}  // namespace Develle
