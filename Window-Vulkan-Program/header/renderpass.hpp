#pragma once
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

class RenderPass {
public:
  void createRenderPass(VkDevice device, VkFormat swapChainImageFormat);

  VkRenderPass renderPass;
};
