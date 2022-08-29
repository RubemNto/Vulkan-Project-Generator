#include <vulkan/vulkan.h>

#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <vector>

struct QueueFamilyIndices {
  std::vector<VkQueueFlags> familyFlags;
  std::vector<std::optional<uint32_t>> familyIndices;
  void setQueueFlags(std::vector<VkQueueFlags> flags);
  VkBool32 isComplete();
};

class Program {
public:
  void run();

private:
  VkInstance pInstance;
  uint32_t pApiVersion;

  VkPhysicalDevice pPhysialDevice;
  // std::map<VkQueueFlags, uint32_t> pPhysicalDeviceQueueFlagCountPair;
  // VkDevice pDevice;

  void createVulkanInstance(uint32_t minAPIVersion);
  VkBool32 checkDeviceSuitability(VkPhysicalDevice device,
                                  uint32_t minAPIVersion,
                                  VkPhysicalDeviceType deviceType,
                                  std::vector<VkQueueFlags> deviceQueueFlags);
  void pickPhysicalDevice();
  void cleanup();
};
