#pragma once
#include "../header/vertexbuffer.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

class GraphicsPipeline {
public:
  void createGraphicsPipeline(VkDevice device, VkExtent2D swapChainExtent,
                              VkRenderPass renderPass,
                              VkDescriptorSetLayout descriptorSetLayout);

  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;
};
