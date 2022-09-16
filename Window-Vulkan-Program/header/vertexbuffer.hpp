#pragma once
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
private:
  uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);

public:
  VkBuffer buffer;
  VkDeviceMemory bufferMemory;

  template <typename Vertex>
  void createVertexBuffer(std::vector<Vertex> vertices,
                          VkPhysicalDevice physicalDevice, VkDevice device,
                          VkDeviceSize bufferSize) {
    VkBufferCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.size = bufferSize;
    createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &createInfo, nullptr, &buffer) != VK_SUCCESS) {
      throw std::runtime_error("failed to create vertex buffer!");
    }
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex =
        findMemoryType(physicalDevice, memoryRequirements.memoryTypeBits,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) !=
        VK_SUCCESS) {
      throw std::runtime_error("failed to allocate vertex buffer memory!");
    }
    vkBindBufferMemory(device, buffer, bufferMemory, 0);

    void *data;
    vkMapMemory(device, bufferMemory, 0, createInfo.size, 0, &data);
    memcpy(data, vertices.data(), (size_t)createInfo.size);
    vkUnmapMemory(device, bufferMemory);
  }
};
