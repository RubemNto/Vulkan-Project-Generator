#include "../header/helperFunctions.hpp"
#include "../header/queuefamilyindices.hpp"
#include "../header/swapchainsupportdetails.hpp"
#include <array>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <algorithm> // Necessary for std::clamp
#include <cstdint>   // Necessary for uint32_t
#include <iostream>
#include <limits> // Necessary for std::numeric_limits
#include <vector>
#include <vulkan/vulkan.h>

class SwapChain {
public:
  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> swapChainImageViews;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  std::vector<VkFramebuffer> swapChainFramebuffers;
  void createFramebuffers(VkDevice device, VkExtent2D extent,
                          VkRenderPass renderPass,
                          std::vector<VkImageView> imageViews,
                          VkImageView depthImageView);
  void createSwapChain(GLFWwindow *window, std::vector<VkQueueFlags> queueFlags,
                       VkPhysicalDevice pDevice, VkDevice lDevice,
                       VkSurfaceKHR surface);
  void createImageViews(VkDevice device);
  VkSurfaceFormatKHR
  pickSurfaceFormat(const std::vector<VkSurfaceFormatKHR> formats);
  VkPresentModeKHR
  pickPresentMode(const std::vector<VkPresentModeKHR> presentModes);
  VkExtent2D pickExtent(GLFWwindow *window,
                        const VkSurfaceCapabilitiesKHR &capabilities);
};
