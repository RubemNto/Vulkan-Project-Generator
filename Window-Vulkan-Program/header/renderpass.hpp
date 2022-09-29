#pragma once
#include "../header/depthBuffer.hpp"
#include <array>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>
class RenderPass {
public:
  void createRenderPass(VkPhysicalDevice physialDevice, VkDevice device,
                        VkFormat swapChainImageFormat, DepthBuffer depthBuffer);

  VkRenderPass renderPass;
};
