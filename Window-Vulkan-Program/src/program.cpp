#include "../header/program.hpp"

void Program::mainLoop(GLFWwindow *window, VkDevice device,
                       VkQueue graphicsQueue, VkQueue presentQueue,
                       VkSwapchainKHR swapChain, VkExtent2D extent,
                       VkRenderPass renderPass, VkPipeline graphicsPipeline) {
  Color bg = color(0.1, 0.1, 0.1, 1.0);
  drawing.setBackgroundColor(bg);
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    drawing.drawFrame(device, graphicsQueue, presentQueue, swapChain,
                      renderPass, extent, graphicsPipeline);
  }
  vkDeviceWaitIdle(device);
}

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
  renderPass.createRenderPass(setup.pDevice, presentation.swapChainImageFormat);
  graphicsPipeline.createGraphicsPipeline(
      setup.pDevice, presentation.swapChainExtent, renderPass.renderPass);
  drawing.createFramebuffers(setup.pDevice, presentation.swapChainExtent,
                             renderPass.renderPass,
                             presentation.swapChainImageViews);
  drawing.createCommandPool(setup.pDevice, setup.pPhysialDevice,
                            setup.deviceQueueFlags, &presentation.surface);
  drawing.createSyncObjects(setup.pDevice);
  drawing.createCommandBuffers(setup.pDevice);
  mainLoop(setup.window, setup.pDevice, setup.pDeviceQueues.at(0),
           setup.pDeviceQueues.at(1), presentation.swapChain,
           presentation.swapChainExtent, renderPass.renderPass,
           graphicsPipeline.graphicsPipeline);
  cleanup();
}

void Program::cleanup() {
  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroySemaphore(setup.pDevice, drawing.renderFinishedSemaphores[i],
                       nullptr);
    vkDestroySemaphore(setup.pDevice, drawing.imageAvailableSemaphores[i],
                       nullptr);
    vkDestroyFence(setup.pDevice, drawing.inFlightFences[i], nullptr);
  }
  vkDestroyCommandPool(setup.pDevice, drawing.commandPool, nullptr);

  for (auto framebuffer : drawing.swapChainFramebuffers) {
    vkDestroyFramebuffer(setup.pDevice, framebuffer, nullptr);
  }
  vkDestroyPipeline(setup.pDevice, graphicsPipeline.graphicsPipeline, nullptr);
  vkDestroyPipelineLayout(setup.pDevice, graphicsPipeline.pipelineLayout,
                          nullptr);
  vkDestroyRenderPass(setup.pDevice, renderPass.renderPass, nullptr);

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
