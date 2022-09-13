#include "../header/vertexbuffer.hpp"

template <typename T>
VkVertexInputBindingDescription getBindingDescription(uint32_t binding) {
  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = binding;
  bindingDescription.stride = sizeof(T);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescription;
}
template <typename T> uint32_t getVertexAttributeCount() {
  if (sizeof(T) == sizeof(VertexColor)) {
    return 2;
  } else if (sizeof(T) == sizeof(VertexColorTexture)) {
    return 3;
  } else if (sizeof(T) == sizeof(VertexNormalTexture)) {
    return 3;
  }
  return 0;
}

template <typename T> std::vector<VkFormat> getVertexAttributeFormats() {
  std::vector<VkFormat> formats;
  if (sizeof(T) == sizeof(VertexColor)) {
    formats.push_back(VK_FORMAT_R32G32B32_SFLOAT);
    formats.push_back(VK_FORMAT_R32G32B32_SFLOAT);
  } else if (sizeof(T) == sizeof(VertexColorTexture)) {
    formats.push_back(VK_FORMAT_R32G32B32_SFLOAT);
    formats.push_back(VK_FORMAT_R32G32B32_SFLOAT);
    formats.push_back(VK_FORMAT_R32G32_SFLOAT);
  } else if (sizeof(T) == sizeof(VertexNormalTexture)) {
    formats.push_back(VK_FORMAT_R32G32B32_SFLOAT);
    formats.push_back(VK_FORMAT_R32G32B32_SFLOAT);
    formats.push_back(VK_FORMAT_R32G32_SFLOAT);
  }
  return formats;
}

template <typename T> std::vector<uint32_t> getVertexAttributeOffsets() {
  std::vector<uint32_t> offsets;
  if (sizeof(T) == sizeof(VertexColor)) {
    offsets.push_back(offsetof(T, VertexColor.position));
    offsets.push_back(offsetof(T, VertexColor.color));
  } else if (sizeof(T) == sizeof(VertexColorTexture)) {
    offsets.push_back(offsetof(T, VertexColorTexture.position));
    offsets.push_back(offsetof(T, VertexColorTexture.color));
    offsets.push_back(offsetof(T, VertexColorTexture.textureCoordinate));
  } else if (sizeof(T) == sizeof(VertexNormalTexture)) {
    offsets.push_back(offsetof(T, VertexNormalTexture.position));
    offsets.push_back(offsetof(T, VertexNormalTexture.normalAxis));
    offsets.push_back(offsetof(T, VertexNormalTexture.textureCoordinate));
  }
  return offsets;
}

template <typename T>
std::vector<VkVertexInputAttributeDescription>
getAttributeDescriptions(uint32_t binding) {
  uint32_t attributeCount = getVertexAttributeCount<T>();
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(
      attributeCount);
  std::vector<VkFormat> formats = getVertexAttributeFormats<T>();
  std::vector<uint32_t> offsets = getVertexAttributeOffsets<T>();

  uint32_t i = 0;
  for (auto &attributeDescription : attributeDescriptions) {
    attributeDescription.binding = binding;
    attributeDescription.location = i;
    attributeDescription.format = formats.at(i);
    attributeDescription.offset = offsets.at(i);
    i++;
  }
}

template <typename T>
void VertexBuffer::createVertexBuffer(std::vector<T> vertices,
                                      VkDevice device) {
  VkBufferCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  createInfo.size = sizeof(vertices[0]) * vertices.size();
  createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device, &createInfo, nullptr, &vertexBuffer) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create vertex buffer");
  } else {
    std::cout << "Created Vertex Buffer" << std::endl;
  }
}
