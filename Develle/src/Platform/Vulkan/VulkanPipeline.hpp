#pragma once
#ifndef VULKANPIPELINE_HPP_
#define VULKANPIPELINE_HPP_

#include <Develle/Renderer/Pipeline.hpp>
#include <vulkan/vulkan.hpp>

namespace Develle {

class VulkanPipeline : public Pipeline {
 public:
  VulkanPipeline(const PipelineOptions &options);

 private:
  vk::Pipeline handle;
  vk::PipelineLayout layout;
};

}  // namespace Develle
#endif  // VULKANPIPELINE_HPP_
