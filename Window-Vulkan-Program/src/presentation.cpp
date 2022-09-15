#include "../header/presentation.hpp"

void Presentation::createSurface(VkInstance instance, GLFWwindow *window) {
  if (glfwCreateWindowSurface(instance, window, nullptr, &surface) !=
      VK_SUCCESS) {
    std::runtime_error("Failed to create GLFW Window Surface!");
  }
  std::cout << "Created GLFW Window Surface" << std::endl;
}
