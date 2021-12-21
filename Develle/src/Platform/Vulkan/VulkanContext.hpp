#pragma once
#ifndef VULKANCONTEXT_HPP_
#define VULKANCONTEXT_HPP_

#include <Develle/Renderer/GraphicsContext.hpp>
#include <optional>
#include <vulkan/vulkan.hpp>

struct SDL_Window;

namespace Develle {

const std::vector<const char*> RequiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool IsComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct SwapchainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

class VulkanContext : public GraphicsContext {
 public:
  VulkanContext(SDL_Window* window);

  void Init() override;
  void SwapBuffers() override {}

  VkDevice GetLogicalDevice() const { return device; }

 private:
  bool UsableDevice(VkPhysicalDevice);
  bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
  QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
  SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
  VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR ChooseSwapchainPresentMode(
      const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);

  SDL_Window* windowHandle;

  VkInstance instance;
  VkSurfaceKHR surface;
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VkQueue graphicsQueue;
  VkQueue presentQueue;

  VkSwapchainKHR swapchain;
  VkFormat swapchainFormat;
  VkExtent2D swapchainExtent;
  std::vector<VkImage> swapchainImages;

  std::vector<VkImageView> swapchainImageViews;
};

}  // namespace Develle

#endif  // VULKANCONTEXT_HPP_
