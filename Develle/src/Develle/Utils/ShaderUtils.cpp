#include "ShaderUtils.hpp"

namespace Utils {

GLenum OpenGLShaderTypeFromString(const std::string &type) {
  if (type == "vertex")
    return GL_VERTEX_SHADER;
  else if (type == "fragment" || type == "pixel")
    return GL_FRAGMENT_SHADER;

  DV_CORE_ASSERT(false, "Unknown shader type");
  return 0;
}

shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
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

const char *GLShaderStageToString(GLenum stage) {
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

VkShaderStageFlagBits VulkanShaderTypeFromString(const std::string &type) {
  if (type == "vertex")
    return VK_SHADER_STAGE_VERTEX_BIT;
  else if (type == "fragment" || type == "pixel")
    return VK_SHADER_STAGE_FRAGMENT_BIT;

  DV_CORE_ASSERT(false, "Unknown shader type");
  return VK_SHADER_STAGE_ALL;
}

shaderc_shader_kind VulkanShaderStageToShaderC(VkShaderStageFlagBits stage) {
  switch (stage) {
    case VK_SHADER_STAGE_VERTEX_BIT:
      return shaderc_glsl_vertex_shader;
    case VK_SHADER_STAGE_FRAGMENT_BIT:
      return shaderc_glsl_fragment_shader;
    default:
      DV_CORE_ASSERT(false, "Unknown shader stage");
      return (shaderc_shader_kind)0;
  }
}

const char *VulkanShaderStageToString(VkShaderStageFlagBits stage) {
  switch (stage) {
    case VK_SHADER_STAGE_VERTEX_BIT:
      return "VK_SHADER_STAGE_VERTEX_BIT";
    case VK_SHADER_STAGE_FRAGMENT_BIT:
      return "VK_SHADER_STAGE_FRAGMENT_BIT";
    default:
      DV_CORE_ASSERT(false, "Unknown shader stage");
      return nullptr;
  }
}

std::string GetCacheDirectory() { return std::string(ASSETS_ROOT) + "/cache/shader/opengl"; }

void CreateCacheDirectoryIfNeeded() {
  std::string cacheDirectory = GetCacheDirectory();
  if (!std::filesystem::exists(cacheDirectory)) std::filesystem::create_directories(cacheDirectory);
}

const char *GLShaderStageCachedOpenGLFileExtension(uint32_t stage) {
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

const char *GLShaderStageCachedVulkanFileExtension(uint32_t stage) {
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

const char *VulkanShaderStageCachedVulkanFileExtension(VkShaderStageFlagBits stage) {
  switch (stage) {
    case VK_SHADER_STAGE_VERTEX_BIT:
      return ".cached_vulkan.vert";
    case VK_SHADER_STAGE_FRAGMENT_BIT:
      return ".cached_vulkan.frag";
    default:
      DV_CORE_ASSERT(false, "Unknown shader stage");
      return "";
  }
}

}  // namespace Utils
