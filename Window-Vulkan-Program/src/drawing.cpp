#include "../header/drawing.hpp"

Color Color::Red() {
  Color color{};
  color.red = 1;
  color.green = 0;
  color.blue = 0;
  color.alpha = 1;
  return color;
}
Color Color::Green() {
  Color color{};
  color.red = 0;
  color.green = 1;
  color.blue = 0;
  color.alpha = 1;
  return color;
}
Color Color::Blue() {
  Color color{};
  color.red = 0;
  color.green = 0;
  color.blue = 1;
  color.alpha = 1;
  return color;
}

Color color(float red, float green, float blue, float alpha) {
  Color color{};
  color.red = red;
  color.green = green;
  color.blue = blue;
  color.alpha = alpha;
  return color;
}

void Drawing::setBackgroundColor(Color color) { backgroundColor = color; }
void Drawing::setBackgroundColor(float r, float g, float b, float a) {
  backgroundColor = color(r, g, b, a);
}

void Drawing::createSyncObjects(VkDevice device) {
  imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    if (vkCreateSemaphore(device, &semaphoreInfo, nullptr,
                          &imageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateSemaphore(device, &semaphoreInfo, nullptr,
                          &renderFinishedSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) !=
            VK_SUCCESS) {
      throw std::runtime_error("failed to create semaphores!");
    }
  }
}

void Drawing::createCommandPool(VkDevice device, VkPhysicalDevice pdevice,
                                std::vector<VkQueueFlags> queueFlags,
                                VkSurfaceKHR *surface) {
  QueueFamilyIndices indices =
      findQueueFamilyIndices(pdevice, queueFlags, surface);
  std::optional<uint32_t> graphicsFamily =
      indices.getFamilyIndice(VK_QUEUE_GRAPHICS_BIT);

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = graphicsFamily.value();
  if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
  }
  std::cout << "Created Command Pool" << std::endl;
}

void Drawing::createCommandBuffers(VkDevice device) {
  commandBuffer.resize(MAX_FRAMES_IN_FLIGHT);
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (uint32_t)commandBuffer.size();

  if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffer.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
  std::cout << "Created Command Buffers" << std::endl;
}

void Drawing::recordElementsCommandBuffer(
    uint32_t indicesCount, VkCommandBuffer commandBuffer, VkBuffer vertexBuffer,
    VkBuffer indexBuffer, uint32_t imageIndex, VkRenderPass renderPass,
    VkExtent2D extent, VkPipeline graphicsPipeline,
    std::vector<VkFramebuffer> swapChainFramebuffers,
    VkPipelineLayout pipelineLayout,
    std::vector<VkDescriptorSet> descriptorSets) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }
  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = renderPass;
  renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = extent;
  // VkClearValue clearColor = {{{backgroundColor.red, backgroundColor.green,
  //                              backgroundColor.blue,
  //                              backgroundColor.alpha}}};

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {{backgroundColor.red, backgroundColor.green,
                           backgroundColor.blue, backgroundColor.alpha}};
  clearValues[1].depthStencil = {1.0f, 0};

  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();
  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphicsPipeline);
  VkBuffer vertexBuffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};

  vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
  vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(extent.width);
  viewport.height = static_cast<float>(extent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = extent;
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipelineLayout, 0, 1, &descriptorSets[currentFrame],
                          0, nullptr);
  vkCmdDrawIndexed(commandBuffer, indicesCount, 1, 0, 0, 0);

  vkCmdEndRenderPass(commandBuffer);
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }

  std::cout << "Recorded Command Buffer" << std::endl;
}

void Drawing::recordVerticesCommandBuffer(
    uint32_t verticesCount, VkCommandBuffer commandBuffer,
    VkBuffer vertexBuffer, uint32_t imageIndex, VkRenderPass renderPass,
    VkExtent2D extent, VkPipeline graphicsPipeline,
    std::vector<VkFramebuffer> swapChainFramebuffers) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }
  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = renderPass;
  renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = extent;
  VkClearValue clearColor = {{{backgroundColor.red, backgroundColor.green,
                               backgroundColor.blue, backgroundColor.alpha}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;
  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphicsPipeline);
  VkBuffer vertexBuffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};

  vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(extent.width);
  viewport.height = static_cast<float>(extent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = extent;
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
  vkCmdDraw(commandBuffer, verticesCount, 1, 0, 0);

  vkCmdEndRenderPass(commandBuffer);
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }

  std::cout << "Recorded Command Buffer" << std::endl;
}
void Drawing::drawElementsFrame(
    VkDevice device, VkQueue graphicsQueue, VkQueue presentQueue,
    VkSwapchainKHR swapChain, std::vector<VkFramebuffer> swapChainFramebuffers,
    uint32_t indicesCount, VkBuffer vertexBuffer, VkBuffer indexBuffer,
    VkRenderPass renderPass, VkExtent2D extent, VkPipeline graphicsPipeline,
    VkPipelineLayout pipelineLayout,
    std::vector<VkDescriptorSet> descriptorSets, UniformBuffer uniformBuffer) {
  vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE,
                  UINT64_MAX);
  vkResetFences(device, 1, &inFlightFences[currentFrame]);
  uint32_t imageIndex;
  vkAcquireNextImageKHR(device, swapChain, UINT64_MAX,
                        imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE,
                        &imageIndex);
  vkResetCommandBuffer(commandBuffer[currentFrame], 0);
  uniformBuffer.updateUniformBuffer(device, currentFrame, 800 / 600.0);
  recordElementsCommandBuffer(indicesCount, commandBuffer[currentFrame],
                              vertexBuffer, indexBuffer, imageIndex, renderPass,
                              extent, graphicsPipeline, swapChainFramebuffers,
                              pipelineLayout, descriptorSets);
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer[currentFrame];
  VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(graphicsQueue, 1, &submitInfo,
                    inFlightFences[currentFrame]) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = {swapChain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  vkQueuePresentKHR(presentQueue, &presentInfo);
  currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Drawing::drawVerticesFrame(
    VkDevice device, VkQueue graphicsQueue, VkQueue presentQueue,
    VkSwapchainKHR swapChain, std::vector<VkFramebuffer> swapChainFramebuffers,
    uint32_t verticesCount, VkBuffer vertexBuffer, VkRenderPass renderPass,
    VkExtent2D extent, VkPipeline graphicsPipeline) {

  vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE,
                  UINT64_MAX);
  vkResetFences(device, 1, &inFlightFences[currentFrame]);
  uint32_t imageIndex;
  vkAcquireNextImageKHR(device, swapChain, UINT64_MAX,
                        imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE,
                        &imageIndex);
  vkResetCommandBuffer(commandBuffer[currentFrame], 0);
  recordVerticesCommandBuffer(verticesCount, commandBuffer[currentFrame],
                              vertexBuffer, imageIndex, renderPass, extent,
                              graphicsPipeline, swapChainFramebuffers);
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer[currentFrame];
  VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(graphicsQueue, 1, &submitInfo,
                    inFlightFences[currentFrame]) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = {swapChain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  vkQueuePresentKHR(presentQueue, &presentInfo);
  currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
