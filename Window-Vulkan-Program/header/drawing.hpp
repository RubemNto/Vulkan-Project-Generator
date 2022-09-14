#pragma once
#include "../header/queuefamilyindices.hpp"
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>
class Drawing {
public:
  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkCommandPool commandPool;
  VkCommandBuffer commandBuffer;
  void createFramebuffers(VkDevice device, VkExtent2D extent,
                          VkRenderPass renderPass,
                          std::vector<VkImageView> imageViews);
  void createCommandPool(VkDevice device, VkPhysicalDevice pdevice,
                         std::vector<VkQueueFlags> queueFlags,
                         VkSurfaceKHR *surface);
  void createCommandBuffer(VkDevice device);
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex,
                           VkRenderPass renderPass, VkExtent2D extent,
                           VkPipeline graphicsPipeline);
};