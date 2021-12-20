#include "VulkanContext.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <Develle/Core/Application.hpp>
#include <set>
#include <vulkan/vulkan.hpp>

namespace Develle {

#define CHECK_VULKAN(fn) \
  VkResult r = fn;       \
  if (r != VK_SUCCESS) throw std::runtime_error(#fn " failed");

VulkanContext::VulkanContext(SDL_Window* window) : windowHandle(window) {
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

void VulkanContext::Init() {
  VkResult result;

  {  // Physical Devices

    uint32_t deviceCount = 0;
    result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    DV_CORE_ASSERT(result == VK_SUCCESS, "Failed to find GPUs with vulkan support");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    result = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    for (auto d : devices) {
      if (UsableDevice(d)) {
        physicalDevice = d;
        break;
      }
    }
    DV_CORE_ASSERT(physicalDevice != VK_NULL_HANDLE, "Failed to find a suitable GPU");
  }

  {
    // Logical Devices & Queues
    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(),
                                              indices.presentFamily.value()};

    VkPhysicalDeviceFeatures features{};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.enabledExtensionCount = RequiredExtensions.size();
    createInfo.ppEnabledExtensionNames = RequiredExtensions.data();

    createInfo.pEnabledFeatures = &features;

    result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
    DV_CORE_ASSERT(result == VK_SUCCESS, "Failed to create logical device");

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
  }

  // Create swapchain
  {
    SwapchainSupportDetails swapchainSupport = QuerySwapchainSupport(physicalDevice);
    VkSurfaceFormatKHR surfaceFormat = ChooseSwapchainSurfaceFormat(swapchainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapchainPresentMode(swapchainSupport.presentModes);
    VkExtent2D extent = ChooseSwapchainExtent(swapchainSupport.capabilities);

    swapchainFormat = surfaceFormat.format;
    swapchainExtent = extent;

    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
    if (swapchainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapchainSupport.capabilities.maxImageCount)
      imageCount = swapchainSupport.capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
    std::array<uint32_t, 2> queueFamilyIndices = {indices.graphicsFamily.value(),
                                                  indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    } else {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    createInfo.preTransform = swapchainSupport.capabilities.currentTransform;

    result = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain);
    DV_CORE_ASSERT(result == VK_SUCCESS, "Failed to create swapchain");

    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());
  }
}

bool VulkanContext::UsableDevice(VkPhysicalDevice device) {
  QueueFamilyIndices indices = FindQueueFamilies(device);

  bool extensionsSupported = CheckDeviceExtensionSupport(device);

  bool swapchainAdequate = false;
  if (extensionsSupported) {
    auto swapchainSupport = QuerySwapchainSupport(device);
    swapchainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
  }

  return indices.IsComplete() && extensionsSupported && swapchainAdequate;
}

bool VulkanContext::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> deviceExtensions(RequiredExtensions.begin(), RequiredExtensions.end());

  for (const auto& extension : availableExtensions) deviceExtensions.erase(extension.extensionName);

  return deviceExtensions.empty();
}

QueueFamilyIndices VulkanContext::FindQueueFamilies(VkPhysicalDevice device) {
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.graphicsFamily = i;

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    if (presentSupport) indices.presentFamily = i;

    if (indices.IsComplete()) break;
    i++;
  }

  return indices;
}

SwapchainSupportDetails VulkanContext::QuerySwapchainSupport(VkPhysicalDevice device) {
  SwapchainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                              details.presentModes.data());
  }

  return details;
}

VkSurfaceFormatKHR VulkanContext::ChooseSwapchainSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats) {
  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      return availableFormat;
  }

  return availableFormats[0];
}

VkPresentModeKHR VulkanContext::ChooseSwapchainPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes) {
  for (const auto& presentMode : availablePresentModes) {
    if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) return presentMode;
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanContext::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
  if (capabilities.currentExtent.width != UINT32_MAX && capabilities.currentExtent.width != 0)
    return capabilities.currentExtent;

  int width, height;
  SDL_Vulkan_GetDrawableSize(windowHandle, &width, &height);

  VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

  actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                                  capabilities.maxImageExtent.width);
  actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                                   capabilities.maxImageExtent.height);

  return actualExtent;
}

}  // namespace Develle
