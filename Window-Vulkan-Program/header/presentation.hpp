#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../header/swapchainsupportdetails.hpp"

#include <iostream>
#include <vector>

class Presentation {
public:
  VkSurfaceKHR surface;
  VkQueue presentQueue;
  void createSurface(VkInstance instance, GLFWwindow *window);
  void createSwapChain();
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
};
