#pragma once
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

namespace Helper {
VkImageView createImageView(VkDevice device, VkImage image, VkFormat format,
                            VkImageAspectFlags aspectFlags);
bool hasStencilComponent(VkFormat format);
void copyBufferToImage(VkDevice device, VkCommandPool commandPool,
                       VkQueue graphicsQueue, VkBuffer buffer, VkImage image,
                       uint32_t width, uint32_t height);
void createBuffer(VkPhysicalDevice physicalDevice, VkDevice device,
                  VkDeviceSize size, VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties, VkBuffer &buffer,
                  VkDeviceMemory &bufferMemory);
uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
                        VkMemoryPropertyFlags properties);
void copyBuffer(VkDevice device, VkQueue graphicsQueue,
                VkCommandPool commandPool, VkBuffer srcBuffer,
                VkBuffer dstBuffer, VkDeviceSize size);
VkCommandBuffer beginSingleTimeCommands(VkDevice device,
                                        VkCommandPool commandPool);
void endSingleTimeCommands(VkDevice device, VkCommandPool commandPool,
                           VkQueue graphicsQueue,
                           VkCommandBuffer commandBuffer);
} // namespace Helper
