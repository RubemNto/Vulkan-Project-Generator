#pragma once
#include <iostream>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

struct QueueFamilyIndices {
  std::vector<VkQueueFlags> familyFlags;
  std::vector<std::optional<uint32_t>> familyIndices;
  std::vector<uint32_t> queueCounts;

  VkBool32 presentable;
  std::optional<uint32_t> presentFamily;

  void setQueueFlags(std::vector<VkQueueFlags> flags);
  VkBool32 isComplete();
};

QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice device,
                                          std::vector<VkQueueFlags> queueFlags,
                                          VkSurfaceKHR *surface);
