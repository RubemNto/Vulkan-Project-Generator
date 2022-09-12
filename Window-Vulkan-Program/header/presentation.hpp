#pragma once
#include "../header/queuefamilyindices.hpp"
#include "../header/swapchainsupportdetails.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <algorithm> // Necessary for std::clamp
#include <cstdint>   // Necessary for uint32_t
#include <iostream>
#include <limits> // Necessary for std::numeric_limits

class Presentation {
public:
  VkSurfaceKHR surface;
  VkSwapchainKHR swapChain;
  void createSurface(VkInstance instance, GLFWwindow *window);
  void createSwapChain(GLFWwindow *window, std::vector<VkQueueFlags> queueFlags,
                       VkPhysicalDevice pDevice, VkDevice lDevice);
  VkSurfaceFormatKHR
  pickSurfaceFormat(const std::vector<VkSurfaceFormatKHR> formats);
  VkPresentModeKHR
  pickPresentMode(const std::vector<VkPresentModeKHR> presentModes);
  VkExtent2D pickExtent(GLFWwindow *window,
                        const VkSurfaceCapabilitiesKHR &capabilities);
};
