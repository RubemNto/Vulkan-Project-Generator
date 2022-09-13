#include "../header/program.hpp"

void Program::run() {
  setup.initWindow();
  setup.createVulkanInstance(VK_MAKE_VERSION(1, 0, 0));
  presentation.createSurface(setup.pInstance, setup.window);
  setup.pickPhysicalDevice(setup.type, setup.deviceQueueFlags,
                           presentation.surface);
  setup.createLogicalDevice(&presentation.surface);
  presentation.createSwapChain(setup.window, setup.deviceQueueFlags,
                               setup.pPhysialDevice, setup.pDevice);
  presentation.createImageViews(setup.pDevice);
  graphicsPipeline.createGraphicsPipeline();
  setup.mainLoop();
  cleanup();
}

void Program::cleanup() {
  for (auto imageView : presentation.swapChainImageViews) {
    vkDestroyImageView(setup.pDevice, imageView, nullptr);
  }
  vkDestroySwapchainKHR(setup.pDevice, presentation.swapChain, nullptr);
  vkDestroyDevice(setup.pDevice, nullptr);
  vkDestroySurfaceKHR(setup.pInstance, presentation.surface, nullptr);
  vkDestroyInstance(setup.pInstance, nullptr);
  glfwDestroyWindow(setup.window);
  glfwTerminate();
}
