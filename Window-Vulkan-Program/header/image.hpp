#include "../header/helperFunctions.hpp"

#include <cstring>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>
class Image {
public:
  VkImage textureImage;
  VkDeviceMemory textureImageMemory;
  void transitionImageLayout(VkDevice device, VkCommandPool commandPool,
                             VkQueue graphicsQueue, VkImage image,
                             VkFormat format, VkImageLayout oldLayout,
                             VkImageLayout newLayout);
  void createImage(VkPhysicalDevice physicalDevice, VkDevice device,
                   uint32_t width, uint32_t height, VkFormat format,
                   VkImageTiling tiling, VkImageUsageFlags usage,
                   VkMemoryPropertyFlags properties, VkImage &image,
                   VkDeviceMemory &imageMemory);
  void createTextureImage(const char *imagePath,
                          VkPhysicalDevice physicalDevice, VkDevice device,
                          VkCommandPool commandPool, VkQueue graphicsQueue);
};
