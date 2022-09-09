#include "../header/program.hpp"

void Program::run() {
  setup.initWindow();
  setup.createVulkanInstance(VK_MAKE_VERSION(1, 0, 0));
  presentation.createSurface(setup.pInstance, setup.window);
<<<<<<< HEAD
  setup.pickPhysicalDevice(setup.type, presentation.surface,
                           setup.deviceQueueFlags);
  setup.createLogicalDevice(&presentation.surface, &presentation.presentQueue);
=======
  setup.pickPhysicalDevice(setup.type, setup.deviceQueueFlags);
  setup.createLogicalDevice();
>>>>>>> parent of 24947f0 (add surface creation)
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
