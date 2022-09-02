#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstring>
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
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

VkBool32 checkValidationLayerSupport();

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class Program {
public:
  void run();

private:
  GLFWwindow *window;
  void initWindow();
  void mainLoop();

  VkInstance pInstance;
  uint32_t pApiVersion;

  VkPhysicalDevice pPhysialDevice;
  const std::vector<VkQueueFlags> deviceQueueFlags = {VK_QUEUE_GRAPHICS_BIT,
                                                      VK_QUEUE_TRANSFER_BIT};
  const VkPhysicalDeviceType type = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
  VkPhysicalDeviceFeatures deviceFeatures = {};

  VkDevice pDevice;
  std::vector<VkQueue> pDeviceQueues;

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
