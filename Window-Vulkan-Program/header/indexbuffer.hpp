#pragma once
#include "../header/helperFunctions.hpp"

#include <array>
#include <glm/glm.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <vulkan/vulkan.h>
class IndexBuffer {
public:
  VkBuffer buffer;
  VkDeviceMemory bufferMemory;

  template <typename IndicesType>
  void createIndexBuffer(VkQueue graphicsQueue, VkCommandPool comamndPool,
                         std::vector<IndicesType> indices,
                         VkPhysicalDevice physicalDevice, VkDevice device,
                         VkDeviceSize bufferSize) {
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    Helper::createBuffer(physicalDevice, device, bufferSize,
                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    Helper::createBuffer(
        physicalDevice, device, bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);
    Helper::copyBuffer(device, graphicsQueue, comamndPool, stagingBuffer,
                       buffer, bufferSize);
    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
  }
};
