#pragma once
#include "../header/graphicspipeline.hpp"
#include "../header/presentation.hpp"
#include "../header/renderpass.hpp"
#include "../header/setup.hpp"

class Program {
public:
  void run();

private:
  Setup setup;
  Presentation presentation;
  GraphicsPipeline graphicsPipeline;
  RenderPass renderPass;
  void cleanup();
};
