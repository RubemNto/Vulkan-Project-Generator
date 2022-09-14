#pragma once
#include "../header/queuefamilyindices.hpp"
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

struct Color {
  float red;
  float green;
  float blue;
  float alpha;

  static Color Red();
  static Color Green();
  static Color Blue();
};

Color color(float red, float green, float blue, float alpha);

const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

class Drawing {
public:
  uint32_t currentFrame = 0;
  Color backgroundColor;
  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffer;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;

  void setBackgroundColor(Color color);
  void setBackgroundColor(float r, float g, float b, float a);

  void createFramebuffers(VkDevice device, VkExtent2D extent,
                          VkRenderPass renderPass,
                          std::vector<VkImageView> imageViews);
  void createCommandPool(VkDevice device, VkPhysicalDevice pdevice,
                         std::vector<VkQueueFlags> queueFlags,
                         VkSurfaceKHR *surface);
  void createCommandBuffers(VkDevice device);
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex,
                           VkRenderPass renderPass, VkExtent2D extent,
                           VkPipeline graphicsPipeline);
  void drawFrame(VkDevice device, VkQueue graphicsQueue, VkQueue presentQueue,
                 VkSwapchainKHR swapChain, VkRenderPass renderPass,
                 VkExtent2D extent, VkPipeline graphicsPipeline);
  void createSyncObjects(VkDevice device);
};
