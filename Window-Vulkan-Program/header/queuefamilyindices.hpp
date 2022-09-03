#include <iostream>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

struct QueueFamilyIndices {
  std::vector<VkQueueFlags> familyFlags;
  std::vector<std::optional<uint32_t>> familyIndices;
  std::vector<uint32_t> queueCounts;

  void setQueueFlags(std::vector<VkQueueFlags> flags);
  VkBool32 isComplete();
};

QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice device,
                                          std::vector<VkQueueFlags> queueFlags);
