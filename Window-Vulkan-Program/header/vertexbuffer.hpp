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

template <typename T>
VkVertexInputBindingDescription getBindingDescription(uint32_t binding);

template <typename T>
std::vector<VkVertexInputAttributeDescription>
getAttributeDescriptions(uint32_t binding);

class VertexBuffer {
private:
  VkBuffer vertexBuffer;

public:
  template <typename T>
  void createVertexBuffer(std::vector<T> vertices, VkDevice device);
};
