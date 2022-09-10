#include "../header/presentation.hpp"

void Presentation::createSurface(VkInstance instance, GLFWwindow *window) {
  if (glfwCreateWindowSurface(instance, window, nullptr, &surface) !=
      VK_SUCCESS) {
    std::runtime_error("Failed to create GLFW Window Surface!");
  }
  std::cout << "Created GLFW Window Surface" << std::endl;
}
void Presentation::createSwapChain(VkPhysicalDevice device) {
  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;

  VkSurfaceCapabilitiesKHR capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);
  if (capabilities.minImageCount >= 3) {
    createInfo.minImageCount = 3;
  } else {
    createInfo.minImageCount = capabilities.minImageCount;
  }
}
