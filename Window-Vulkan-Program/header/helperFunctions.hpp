#pragma once
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

namespace Helper {
void createBuffer(VkPhysicalDevice physicalDevice, VkDevice device,
                  VkDeviceSize size, VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties, VkBuffer &buffer,
                  VkDeviceMemory &bufferMemory);
uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
                        VkMemoryPropertyFlags properties);
void copyBuffer(VkDevice device, VkQueue graphicsQueue,
                VkCommandPool commandPool, VkBuffer srcBuffer,
                VkBuffer dstBuffer, VkDeviceSize size);
} // namespace Helper
