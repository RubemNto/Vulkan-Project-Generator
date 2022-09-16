#pragma once
#include "../header/helperFunctions.hpp"

#include <array>
#include <glm/glm.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <vulkan/vulkan.h>

struct VertexColor {
  glm::vec3 position;
  glm::vec3 color;

  static std::array<VkVertexInputAttributeDescription, 2>
  getAttributeDescriptions(uint32_t binding,
                           std::array<uint32_t, 2> locations) {
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
    attributeDescriptions[0].binding = binding;
    attributeDescriptions[0].location = locations.at(0);
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(VertexColor, position);

    attributeDescriptions[1].binding = binding;
    attributeDescriptions[1].location = locations.at(1);
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(VertexColor, color);
    return attributeDescriptions;
  }
};
struct VertexColorTexture {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 textureCoordinate;
  static std::array<VkVertexInputAttributeDescription, 3>
  getAttributeDescriptions(uint32_t binding,
                           std::array<uint32_t, 3> locations) {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
    attributeDescriptions[0].binding = binding;
    attributeDescriptions[0].location = locations.at(0);
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(VertexColorTexture, position);

    attributeDescriptions[1].binding = binding;
    attributeDescriptions[1].location = locations.at(1);
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(VertexColorTexture, color);

    attributeDescriptions[2].binding = binding;
    attributeDescriptions[2].location = locations.at(2);
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset =
        offsetof(VertexColorTexture, textureCoordinate);
    return attributeDescriptions;
  }
};

struct VertexNormalTexture {
  glm::vec3 position;
  glm::vec3 normalAxis;
  glm::vec2 textureCoordinate;
  static std::array<VkVertexInputAttributeDescription, 3>
  getAttributeDescriptions(uint32_t binding,
                           std::array<uint32_t, 3> locations) {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
    attributeDescriptions[0].binding = binding;
    attributeDescriptions[0].location = locations.at(0);
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(VertexNormalTexture, position);

    attributeDescriptions[1].binding = binding;
    attributeDescriptions[1].location = locations.at(1);
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(VertexNormalTexture, normalAxis);

    attributeDescriptions[2].binding = binding;
    attributeDescriptions[2].location = locations.at(2);
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset =
        offsetof(VertexNormalTexture, textureCoordinate);
    return attributeDescriptions;
  }
};

template <typename Vertex>
VkVertexInputBindingDescription getVertexBindingDescription(uint32_t binding) {
  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = binding;
  bindingDescription.stride = sizeof(Vertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescription;
}

class VertexBuffer {
public:
  VkBuffer buffer;
  VkDeviceMemory bufferMemory;

  template <typename Vertex>
  void createVertexBuffer(VkQueue graphicsQueue, VkCommandPool comamndPool,
                          std::vector<Vertex> vertices,
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
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    Helper::createBuffer(
        physicalDevice, device, bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);
    Helper::copyBuffer(device, graphicsQueue, comamndPool, stagingBuffer,
                       buffer, bufferSize);
    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
  }
};
