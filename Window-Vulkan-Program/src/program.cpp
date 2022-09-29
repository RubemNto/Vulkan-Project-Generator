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
        indexBuffer.buffer, renderPass.renderPass, swapChain.swapChainExtent,
        graphicsPipeline.graphicsPipeline, graphicsPipeline.pipelineLayout,
        uniformBuffer.descriptorSets, uniformBuffer);
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
  renderPass.createRenderPass(setup.pPhysialDevice, setup.pDevice,
                              swapChain.swapChainImageFormat, depthBuffer);
  uniformBuffer.createDescriptorSetLayout(setup.pDevice);
  graphicsPipeline.createGraphicsPipeline(
      setup.pDevice, swapChain.swapChainExtent, renderPass.renderPass,
      uniformBuffer.descriptorSetLayout);
  drawing.createCommandPool(setup.pDevice, setup.pPhysialDevice,
                            setup.deviceQueueFlags, &presentation.surface);
  depthBuffer.createDepthResources(
      setup.pPhysialDevice, setup.pDevice, image, swapChain.swapChainExtent,
      drawing.commandPool, setup.pDeviceQueues.at(0));
  swapChain.createFramebuffers(
      setup.pDevice, swapChain.swapChainExtent, renderPass.renderPass,
      swapChain.swapChainImageViews, depthBuffer.depthImageView);
  image.createTextureImage("assets/images/Square_funny.png",
                           setup.pPhysialDevice, setup.pDevice,
                           drawing.commandPool, setup.pDeviceQueues.at(0));
  image.createTextureImageView(setup.pDevice);
  image.createTextureSampler(setup.pDevice, setup.pPhysialDevice);
  vertexBuffer.createVertexBuffer<VertexColorTexture>(
      setup.pDeviceQueues.at(0), drawing.commandPool, vertices,
      setup.pPhysialDevice, setup.pDevice,
      sizeof(vertices[0]) * vertices.size());
  indexBuffer.createIndexBuffer<uint32_t>(
      setup.pDeviceQueues.at(0), drawing.commandPool, indices,
      setup.pPhysialDevice, setup.pDevice, sizeof(indices[0]) * indices.size());
  uniformBuffer.createUniformBuffer(setup.pPhysialDevice, setup.pDevice,
                                    sizeof(UniformBufferObject),
                                    MAX_FRAMES_IN_FLIGHT);
  uniformBuffer.createDescriptorPool(setup.pDevice, MAX_FRAMES_IN_FLIGHT);
  uniformBuffer.createDescriptorSets(setup.pDevice, MAX_FRAMES_IN_FLIGHT,
                                     image.textureImageView,
                                     image.textureSampler);

  drawing.createCommandBuffers(setup.pDevice);
  drawing.createSyncObjects(setup.pDevice);
  mainLoop();
  cleanup();
}

void Program::cleanup() {
  vkDestroyImage(setup.pDevice, image.textureImage, nullptr);
  vkFreeMemory(setup.pDevice, image.textureImageMemory, nullptr);
  vkDestroyBuffer(setup.pDevice, indexBuffer.buffer, nullptr);
  vkFreeMemory(setup.pDevice, indexBuffer.bufferMemory, nullptr);
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
  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroyBuffer(setup.pDevice, uniformBuffer.uniformBuffers[i], nullptr);
    vkFreeMemory(setup.pDevice, uniformBuffer.uniformBuffersMemory[i], nullptr);
  }
  vkDestroyDescriptorSetLayout(setup.pDevice, uniformBuffer.descriptorSetLayout,
                               nullptr);
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
