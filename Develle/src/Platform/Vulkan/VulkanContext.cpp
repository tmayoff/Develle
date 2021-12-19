#include "VulkanContext.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <Develle/Core/Application.hpp>
#include <vulkan/vulkan.hpp>

namespace Develle {

#define CHECK_VULKAN(fn) \
  VkResult r = fn;       \
  if (r != VK_SUCCESS) throw std::runtime_error(#fn " failed");

VulkanContext::VulkanContext(SDL_Window* window) {
  uint32_t extensionCount;
  bool success = SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
  DV_CORE_ASSERT(success, "Failed to get required extension count");

  std::vector<const char*> requiredExtension(extensionCount);
  success = SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, requiredExtension.data());
  DV_CORE_ASSERT(success, "Failed to get required extensions");

  const std::array<const char*, 1> validationLayers = {"VK_LAYER_KHRONOS_validation"};

  // Vulkan Instance
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Develle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Develle";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = requiredExtension.size();
  createInfo.ppEnabledExtensionNames = requiredExtension.data();
  createInfo.enabledLayerCount = validationLayers.size();
  createInfo.ppEnabledLayerNames = validationLayers.data();

  VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
  DV_CORE_ASSERT(result == VK_SUCCESS, "vkCreateInstance Failed");

  // Create surface
  success = SDL_Vulkan_CreateSurface(window, instance, &surface);
  DV_CORE_ASSERT(success == SDL_TRUE,
                 "failed to create VK Surface: " + std::string(SDL_GetError()));
}

void VulkanContext::Init() {}

}  // namespace Develle
