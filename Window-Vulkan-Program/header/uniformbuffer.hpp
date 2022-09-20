#pragma once
#include "../header/helperFunctions.hpp"
#include <array>
#include <chrono>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <vulkan/vulkan.h>

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};
class UniformBuffer {
public:
  std::vector<VkBuffer> uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;
  VkDescriptorSetLayout descriptorSetLayout;
  VkPipelineLayout pipelineLayout;
  VkDescriptorPool descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;

  void createDescriptorPool(VkDevice device, uint32_t descriptorCount);
  void createDescriptorSets(VkDevice device, uint32_t descriptorSetCount);

  void createDescriptorSetLayout(VkDevice device);
  void updateUniformBuffer(VkDevice device, uint32_t currentImage,
                           float aspectRatio);
  void createUniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device,
                           VkDeviceSize bufferSize, uint32_t bufferCount);
};
