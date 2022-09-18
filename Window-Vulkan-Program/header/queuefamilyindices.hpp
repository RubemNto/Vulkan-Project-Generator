#pragma once
#include <iostream>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

struct GraphicsPresentQueueFramilyIndices {
  std::optional<uint32_t> graphicsFamilyIndice;
  uint32_t graphicsFamilyQueueCount;
  std::optional<uint32_t> presentFamilyIndice;
  uint32_t presentFamilyQueueCount;

  VkBool32 isComplete();
};

struct ComputeQueueFamilyIndices {
  std::optional<uint32_t> computeFamilyIndice;
  uint32_t computeFamilyQueueCount;

  VkBool32 isComplete();
};
struct TransferQueueFamilyIndices {
  std::optional<uint32_t> transferFamilyIndice;
  uint32_t transferFamilyQueueCount;

  VkBool32 isComplete();
};
struct ProtectedQueueFamilyIndices {
  std::optional<uint32_t> protectedFamilyIndice;
  uint32_t protectedFamilyQueueCount;

  VkBool32 isComplete();
};

struct QueueFamilyIndices {
  std::vector<VkQueueFlags> familyFlags;
  std::vector<std::optional<uint32_t>> familyIndices;
  std::vector<uint32_t> queueCounts;

  VkBool32 presentable;
  std::optional<uint32_t> presentFamily;

  void setQueueFlags(std::vector<VkQueueFlags> flags);
  std::optional<uint32_t> getFamilyIndice(VkQueueFlags flag);
  VkBool32 isComplete();
};

GraphicsPresentQueueFramilyIndices
findGraphicsPresentQueueFamilyIndices(VkPhysicalDevice device,
                                      VkSurfaceKHR *surface);

TransferQueueFamilyIndices
findTransferQueueFamilyIndices(VkPhysicalDevice device);

ComputeQueueFamilyIndices
findComputeQueueFamilyIndices(VkPhysicalDevice device);

ProtectedQueueFamilyIndices
findProtectedQueueFamilyIndices(VkPhysicalDevice device);

QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice device,
                                          std::vector<VkQueueFlags> queueFlags,
                                          VkSurfaceKHR *surface);
