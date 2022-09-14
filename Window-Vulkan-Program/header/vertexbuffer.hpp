#pragma once
#include <array>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>
struct VertexColor {
  glm::vec3 position;
  glm::vec3 color;
};
struct VertexColorTexture {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 textureCoordinate;
};
struct VertexNormalTexture {
  glm::vec3 position;
  glm::vec3 normalAxis;
  glm::vec2 textureCoordinate;
};

class VertexBuffer {
private:
  VkBuffer vertexBuffer;

public:
  void createVertexBuffer();
};
