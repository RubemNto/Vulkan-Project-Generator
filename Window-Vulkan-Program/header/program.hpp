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
  std::vector<uint32_t> queueCounts;
  void setQueueFlags(std::vector<VkQueueFlags> flags);
  VkBool32 isComplete();
};

class Program {
public:
  void run();

private:
  VkInstance pInstance;
  uint32_t pApiVersion;

  const std::vector<VkQueueFlags> deviceQueueFlags = {VK_QUEUE_GRAPHICS_BIT,
                                                      VK_QUEUE_TRANSFER_BIT};
  const VkPhysicalDeviceType type = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
  const std::vector<VkPhysicalDeviceFeatures> deviceFeatures = {};

  VkPhysicalDevice pPhysialDevice;
  VkDevice pDevice;

  void createVulkanInstance(uint32_t minAPIVersion);
  VkBool32 checkDeviceSuitability(VkPhysicalDevice device,
                                  uint32_t minAPIVersion,
                                  VkPhysicalDeviceType deviceType,
                                  std::vector<VkQueueFlags> deviceQueueFlags);
  void pickPhysicalDevice(VkPhysicalDeviceType type,
                          std::vector<VkQueueFlags> deviceQueueFlags);

  void createLogicalDevice();
  void cleanup();
};
