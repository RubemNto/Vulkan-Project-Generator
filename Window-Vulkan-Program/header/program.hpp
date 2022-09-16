#pragma once
#include "../header/drawing.hpp"
#include "../header/graphicspipeline.hpp"
#include "../header/presentation.hpp"
#include "../header/renderpass.hpp"
#include "../header/setup.hpp"
#include "../header/swapchain.hpp"
#include "../header/vertexbuffer.hpp"
class Program {
public:
  const std::vector<VertexColor> vertices = {
      {{0.0f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
      {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}};
  void run();
  void mainLoop(GLFWwindow *window, VkDevice device, VkQueue graphicsQueue,
                VkQueue presentQueue, VkSwapchainKHR swapChain,
                VkExtent2D extent, VkRenderPass renderPass,
                VkPipeline graphicsPipeline);

private:
  VertexBuffer vertexBuffer;
  SwapChain swapChain;
  Setup setup;
  Presentation presentation;
  GraphicsPipeline graphicsPipeline;
  RenderPass renderPass;
  Drawing drawing;
  void cleanup();
};
