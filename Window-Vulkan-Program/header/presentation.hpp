#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
class Presentation {
public:
  VkSurfaceKHR surface;
  void createSurface(VkInstance instance, GLFWwindow *window);
};
