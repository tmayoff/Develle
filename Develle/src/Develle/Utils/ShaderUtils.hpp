#pragma once
#ifndef SHADERUTILS_HPP_
#define SHADERUTILS_HPP_

#include <glad/glad.h>

#include <shaderc/shaderc.hpp>
#include <vulkan/vulkan.hpp>

namespace Utils {

GLenum OpenGLShaderTypeFromString(const std::string &type);
shaderc_shader_kind GLShaderStageToShaderC(GLenum stage);
const char *GLShaderStageToString(GLenum stage);

VkShaderStageFlagBits VulkanShaderTypeFromString(const std::string &type);
shaderc_shader_kind VulkanShaderStageToShaderC(VkShaderStageFlagBits stage);
const char *VulkanShaderStageToString(VkShaderStageFlagBits stage);

std::string GetCacheDirectory();
void CreateCacheDirectoryIfNeeded();

const char *GLShaderStageCachedOpenGLFileExtension(uint32_t stage);
const char *GLShaderStageCachedVulkanFileExtension(uint32_t stage);

const char *VulkanShaderStageCachedVulkanFileExtension(VkShaderStageFlagBits stage);

}  // namespace Utils
#endif  // SHADERUTILS_HPP_
