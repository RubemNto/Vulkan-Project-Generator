#include "../header/program.hpp"

void Program::run() {
  setup.initWindow();
  setup.createVulkanInstance(VK_MAKE_VERSION(1, 0, 0));
  presentation.createSurface(setup.pInstance, setup.window);
  setup.pickPhysicalDevice(setup.type, setup.deviceQueueFlags);
  setup.createLogicalDevice();
  setup.mainLoop();
  cleanup();
}

void Program::cleanup() {
  vkDestroyDevice(setup.pDevice, nullptr);
  vkDestroySurfaceKHR(setup.pInstance, presentation.surface, nullptr);
  vkDestroyInstance(setup.pInstance, nullptr);
  glfwDestroyWindow(setup.window);
  glfwTerminate();
}
