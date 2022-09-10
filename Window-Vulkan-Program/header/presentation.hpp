#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
class Presentation {
public:
  VkSurfaceKHR surface;
  VkSwapchainKHR swapChain;
  void createSurface(VkInstance instance, GLFWwindow *window);
  void createSwapChain(VkPhysicalDevice device);
};
