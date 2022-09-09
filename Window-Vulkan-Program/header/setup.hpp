#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../header/queuefamilyindices.hpp"
#include "../header/swapchainsupportdetails.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

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

class Setup {
public:
  GLFWwindow *window;
  void initWindow();
  void mainLoop();

  VkInstance pInstance;
  uint32_t pApiVersion;

  VkPhysicalDevice pPhysialDevice;
  const std::vector<VkQueueFlags> deviceQueueFlags = {VK_QUEUE_GRAPHICS_BIT,
                                                      VK_QUEUE_TRANSFER_BIT};
  const VkPhysicalDeviceType type = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
  const std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  VkPhysicalDeviceFeatures deviceFeatures = {};

  VkDevice pDevice;
  std::vector<VkQueue> pDeviceQueues;

  void createVulkanInstance(uint32_t minAPIVersion);
  VkBool32 checkDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface,
                                  uint32_t minAPIVersion,
                                  VkPhysicalDeviceType deviceType,
                                  std::vector<VkQueueFlags> deviceQueueFlags);
  VkBool32 checkDeviceExtensionSupport(VkPhysicalDevice device);
  void pickPhysicalDevice(VkPhysicalDeviceType type, VkSurfaceKHR surface,
                          std::vector<VkQueueFlags> deviceQueueFlags);

<<<<<<< HEAD
  void createLogicalDevice(VkSurfaceKHR *surface, VkQueue *presentationQueue);
=======
  void createLogicalDevice();
>>>>>>> parent of 24947f0 (add surface creation)
};
