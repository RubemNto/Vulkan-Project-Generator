#pragma once
#include "../header/queuefamilyindices.hpp"
#include "../header/uniformbuffer.hpp"
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
  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffer;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;

  void setBackgroundColor(Color color);
  void setBackgroundColor(float r, float g, float b, float a);

  void createCommandPool(VkDevice device, VkPhysicalDevice pdevice,
                         std::vector<VkQueueFlags> queueFlags,
                         VkSurfaceKHR *surface);
  void createCommandBuffers(VkDevice device);

  void recordVerticesCommandBuffer(
      uint32_t verticesCount, VkCommandBuffer commandBuffer,
      VkBuffer vertexBuffer, uint32_t imageIndex, VkRenderPass renderPass,
      VkExtent2D extent, VkPipeline graphicsPipeline,
      std::vector<VkFramebuffer> swapChainFramebuffers);

  void drawVerticesFrame(VkDevice device, VkQueue graphicsQueue,
                         VkQueue presentQueue, VkSwapchainKHR swapChain,
                         std::vector<VkFramebuffer> swapChainFramebuffers,
                         uint32_t verticesCount, VkBuffer vertexBuffer,
                         VkRenderPass renderPass, VkExtent2D extent,
                         VkPipeline graphicsPipeline);

  void recordElementsCommandBuffer(
      uint32_t indicesCount, VkCommandBuffer commandBuffer,
      VkBuffer vertexBuffer, VkBuffer indexBuffer, uint32_t imageIndex,
      VkRenderPass renderPass, VkExtent2D extent, VkPipeline graphicsPipeline,
      std::vector<VkFramebuffer> swapChainFramebuffers,
      VkPipelineLayout pipelineLayout,
      std::vector<VkDescriptorSet> descriptorSets);

  void drawElementsFrame(VkDevice device, VkQueue graphicsQueue,
                         VkQueue presentQueue, VkSwapchainKHR swapChain,
                         std::vector<VkFramebuffer> swapChainFramebuffers,
                         uint32_t indicesCount, VkBuffer vertexBuffer,
                         VkBuffer indexBuffer, VkRenderPass renderPass,
                         VkExtent2D extent, VkPipeline graphicsPipeline,
                         VkPipelineLayout pipelineLayout,
                         std::vector<VkDescriptorSet> descriptorSets,
                         UniformBuffer uniformBuffer);

  void createSyncObjects(VkDevice device);
};
