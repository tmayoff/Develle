#pragma once
#ifndef VULKANSURFACE_HPP_
#define VULKANSURFACE_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <Develle/Core/Application.hpp>
#include <vulkan/vulkan.hpp>

namespace Develle {

vk::SurfaceKHR CreateSurfaceKHR(SDL_Window* window, const vk::Instance& instance) {
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  SDL_Vulkan_CreateSurface(window, instance, &surface);
  return surface;
}

}  // namespace Develle
#endif  // VULKANSURFACE_HPP_
