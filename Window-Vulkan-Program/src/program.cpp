#include "../header/program.hpp"

void Program::mainLoop() {
  Color bg = color(0.0, 0.1, 0.1, 1.0);
  drawing.setBackgroundColor(bg);
  while (!glfwWindowShouldClose(setup.window)) {
    glfwPollEvents();
    drawing.drawElementsFrame(
        setup.pDevice, setup.pDeviceQueues.at(0), setup.pDeviceQueues.at(1),
        swapChain.swapChain, swapChain.swapChainFramebuffers,
        static_cast<uint32_t>(indices.size()), vertexBuffer.buffer,
        indexbuffer.buffer, renderPass.renderPass, swapChain.swapChainExtent,
        graphicsPipeline.graphicsPipeline);
  }
  vkDeviceWaitIdle(setup.pDevice);
}

void Program::run() {
  setup.initWindow();
  setup.createVulkanInstance(VK_MAKE_VERSION(1, 0, 0));
  presentation.createSurface(setup.pInstance, setup.window);
  setup.pickPhysicalDevice(setup.type, setup.deviceQueueFlags,
                           presentation.surface);
  setup.createLogicalDevice(&presentation.surface);
  swapChain.createSwapChain(setup.window, setup.deviceQueueFlags,
                            setup.pPhysialDevice, setup.pDevice,
                            presentation.surface);
  swapChain.createImageViews(setup.pDevice);
  renderPass.createRenderPass(setup.pDevice, swapChain.swapChainImageFormat);
  graphicsPipeline.createGraphicsPipeline(
      setup.pDevice, swapChain.swapChainExtent, renderPass.renderPass);
  swapChain.createFramebuffers(setup.pDevice, swapChain.swapChainExtent,
                               renderPass.renderPass,
                               swapChain.swapChainImageViews);
  drawing.createCommandPool(setup.pDevice, setup.pPhysialDevice,
                            setup.deviceQueueFlags, &presentation.surface);
  vertexBuffer.createVertexBuffer<VertexColor>(
      setup.pDeviceQueues.at(0), drawing.commandPool, vertices,
      setup.pPhysialDevice, setup.pDevice,
      sizeof(vertices[0]) * vertices.size());
  indexbuffer.createIndexBuffer<uint32_t>(
      setup.pDeviceQueues.at(0), drawing.commandPool, indices,
      setup.pPhysialDevice, setup.pDevice, sizeof(indices[0]) * indices.size());
  drawing.createCommandBuffers(setup.pDevice);
  drawing.createSyncObjects(setup.pDevice);
  mainLoop();
  cleanup();
}

void Program::cleanup() {
  vkDestroyBuffer(setup.pDevice, indexbuffer.buffer, nullptr);
  vkFreeMemory(setup.pDevice, indexbuffer.bufferMemory, nullptr);
  vkDestroyBuffer(setup.pDevice, vertexBuffer.buffer, nullptr);
  vkFreeMemory(setup.pDevice, vertexBuffer.bufferMemory, nullptr);
  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroySemaphore(setup.pDevice, drawing.renderFinishedSemaphores[i],
                       nullptr);
    vkDestroySemaphore(setup.pDevice, drawing.imageAvailableSemaphores[i],
                       nullptr);
    vkDestroyFence(setup.pDevice, drawing.inFlightFences[i], nullptr);
  }
  vkDestroyCommandPool(setup.pDevice, drawing.commandPool, nullptr);

  for (auto framebuffer : swapChain.swapChainFramebuffers) {
    vkDestroyFramebuffer(setup.pDevice, framebuffer, nullptr);
  }
  vkDestroyPipeline(setup.pDevice, graphicsPipeline.graphicsPipeline, nullptr);
  vkDestroyPipelineLayout(setup.pDevice, graphicsPipeline.pipelineLayout,
                          nullptr);
  vkDestroyRenderPass(setup.pDevice, renderPass.renderPass, nullptr);

  for (auto imageView : swapChain.swapChainImageViews) {
    vkDestroyImageView(setup.pDevice, imageView, nullptr);
  }
  vkDestroySwapchainKHR(setup.pDevice, swapChain.swapChain, nullptr);
  vkDestroyDevice(setup.pDevice, nullptr);
  vkDestroySurfaceKHR(setup.pInstance, presentation.surface, nullptr);
  vkDestroyInstance(setup.pInstance, nullptr);
  glfwDestroyWindow(setup.window);
  glfwTerminate();
}
