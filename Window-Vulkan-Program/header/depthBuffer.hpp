#pragma once
#include "../header/helperFunctions.hpp"
#include "../header/image.hpp"
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

class DepthBuffer {
private:
  VkFormat findSupportedFormat(VkPhysicalDevice physialDevice,
                               const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);

public:
  VkImage depthImage;
  VkDeviceMemory depthImageMemory;
  VkImageView depthImageView;
  void createDepthResources(VkPhysicalDevice physialDevice, VkDevice device,
                            Image image, VkExtent2D swapChainExtent,
                            VkCommandPool commandPool, VkQueue graphicsQueue);
  VkFormat findDepthFormat(VkPhysicalDevice physialDevice);
};
