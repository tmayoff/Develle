#ifndef VULKANCONTEXT_HPP_
#define VULKANCONTEXT_HPP_

#include <Develle/Renderer/GraphicsContext.hpp>
#include <vulkan/vulkan.hpp>

struct SDL_Window;

namespace Develle {

class VulkanContext : public GraphicsContext {
 public:
  VulkanContext(SDL_Window* window);

  void Init() override;
  void SwapBuffers() override {}

 private:
  VkInstance instance;
  VkSurfaceKHR surface;
};

}  // namespace Develle
#endif  // VULKANCONTEXT_HPP_
