#pragma once
#include "../header/depthBuffer.hpp"
#include "../header/drawing.hpp"
#include "../header/graphicspipeline.hpp"
#include "../header/image.hpp"
#include "../header/indexbuffer.hpp"
#include "../header/presentation.hpp"
#include "../header/renderpass.hpp"
#include "../header/setup.hpp"
#include "../header/swapchain.hpp"
#include "../header/uniformbuffer.hpp"
#include "../header/vertexbuffer.hpp"
class Program {
public:
  const std::vector<VertexColorTexture> vertices = {
      {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
      {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
      {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

      {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
      {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
      {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
  };

  const std::vector<uint32_t> indices = {
      0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4,
  };
  void run();
  void mainLoop();

private:
  DepthBuffer depthBuffer;
  Image image;
  VertexBuffer vertexBuffer;
  IndexBuffer indexBuffer;
  UniformBuffer uniformBuffer;
  SwapChain swapChain;
  Setup setup;
  Presentation presentation;
  GraphicsPipeline graphicsPipeline;
  RenderPass renderPass;
  Drawing drawing;
  void cleanup();
};
