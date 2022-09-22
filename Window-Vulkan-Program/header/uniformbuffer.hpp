#pragma once
#include "../header/helperFunctions.hpp"
#include <array>
#include <chrono>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <vulkan/vulkan.h>

struct UniformBufferObject {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};
class UniformBuffer {
public:
  std::vector<VkBuffer> uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;
  VkDescriptorSetLayout descriptorSetLayout;
  VkPipelineLayout pipelineLayout;
  VkDescriptorPool descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;

  void createDescriptorPool(VkDevice device, uint32_t framesInFlight);
  void createDescriptorSets(VkDevice device, uint32_t descriptorSetCount,
                            VkImageView textureImageView,
                            VkSampler textureSampler);

  void createDescriptorSetLayout(VkDevice device);
  void updateUniformBuffer(VkDevice device, uint32_t currentImage,
                           float aspectRatio);
  void createUniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device,
                           VkDeviceSize bufferSize, uint32_t bufferCount);
};
