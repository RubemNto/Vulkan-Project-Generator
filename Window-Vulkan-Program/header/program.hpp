#pragma once
#include "../header/drawing.hpp"
#include "../header/graphicspipeline.hpp"
#include "../header/presentation.hpp"
#include "../header/renderpass.hpp"
#include "../header/setup.hpp"
#include "../header/swapchain.hpp"
class Program {
public:
  void run();
  void mainLoop(GLFWwindow *window, VkDevice device, VkQueue graphicsQueue,
                VkQueue presentQueue, VkSwapchainKHR swapChain,
                VkExtent2D extent, VkRenderPass renderPass,
                VkPipeline graphicsPipeline);

private:
  SwapChain swapChain;
  Setup setup;
  Presentation presentation;
  GraphicsPipeline graphicsPipeline;
  RenderPass renderPass;
  Drawing drawing;
  void cleanup();
};
