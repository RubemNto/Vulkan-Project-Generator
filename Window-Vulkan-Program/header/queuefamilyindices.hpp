#pragma once
#include <iostream>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

struct GraphicsPresentQueueFramilyIndices {
  std::optional<uint32_t> graphicsFamilyIndice;
  uint32_t graphicsFamilyQueueCount;

  std::optional<uint32_t> presentFamilyIndice;
  uint32_t presentFamilyQueueCount;

  VkBool32 isComplete();
};

struct QueueFamilyIndices {
  std::vector<VkQueueFlags> familyFlags;
  std::vector<std::optional<uint32_t>> familyIndices;
  std::vector<uint32_t> queueCounts;

  VkBool32 presentable;
  std::optional<uint32_t> presentFamily;

  void setQueueFlags(std::vector<VkQueueFlags> flags);
  std::optional<uint32_t> getFamilyIndice(VkQueueFlags flag);
  VkBool32 isComplete();
};

GraphicsPresentQueueFramilyIndices
findQueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR *surface);

QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice device,
                                          std::vector<VkQueueFlags> queueFlags,
                                          VkSurfaceKHR *surface);
