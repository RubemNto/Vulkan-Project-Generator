#include "../header/queuefamilyindices.hpp"

VkBool32 ComputeQueueFamilyIndices::isComplete() {
  return computeFamilyIndice.has_value();
}
VkBool32 GraphicsPresentQueueFramilyIndices::isComplete() {
  return graphicsFamilyIndice.has_value() && presentFamilyIndice.has_value();
}
VkBool32 TransferQueueFamilyIndices::isComplete() {
  return transferFamilyIndice.has_value();
}
VkBool32 ProtectedQueueFamilyIndices::isComplete() {
  return protectedFamilyIndice.has_value();
}

ProtectedQueueFamilyIndices
findProtectedQueueFamilyIndices(VkPhysicalDevice device) {
  uint32_t queueFamilyPropertiesCount;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           nullptr);
  if (queueFamilyPropertiesCount <= 0) {
    std::runtime_error("Failed to get Queue Families from Physcial Device");
  }

  ProtectedQueueFamilyIndices queueFamilyIndices;

  std::vector<VkQueueFamilyProperties> queueFamilyProperties;
  queueFamilyProperties.resize(queueFamilyPropertiesCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           queueFamilyProperties.data());
  uint32_t i = 0;
  for (const auto &queueFamilyProperty : queueFamilyProperties) {
    if (queueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) {
      queueFamilyIndices.protectedFamilyIndice = i;
      queueFamilyIndices.protectedFamilyQueueCount =
          queueFamilyProperty.queueCount;
    }
    if (queueFamilyIndices.isComplete()) {
      break;
    }
    i++;
  }
  return queueFamilyIndices;
}

ComputeQueueFamilyIndices
findComputeQueueFamilyIndices(VkPhysicalDevice device) {
  uint32_t queueFamilyPropertiesCount;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           nullptr);
  if (queueFamilyPropertiesCount <= 0) {
    std::runtime_error("Failed to get Queue Families from Physcial Device");
  }

  ComputeQueueFamilyIndices queueFamilyIndices;

  std::vector<VkQueueFamilyProperties> queueFamilyProperties;
  queueFamilyProperties.resize(queueFamilyPropertiesCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           queueFamilyProperties.data());
  uint32_t i = 0;
  for (const auto &queueFamilyProperty : queueFamilyProperties) {
    if (queueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) {
      queueFamilyIndices.computeFamilyIndice = i;
      queueFamilyIndices.computeFamilyQueueCount =
          queueFamilyProperty.queueCount;
    }
    if (queueFamilyIndices.isComplete()) {
      break;
    }
    i++;
  }
  return queueFamilyIndices;
}

TransferQueueFamilyIndices
findTransferQueueFamilyIndices(VkPhysicalDevice device) {
  uint32_t queueFamilyPropertiesCount;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           nullptr);
  if (queueFamilyPropertiesCount <= 0) {
    std::runtime_error("Failed to get Queue Families from Physcial Device");
  }

  TransferQueueFamilyIndices queueFamilyIndices;

  std::vector<VkQueueFamilyProperties> queueFamilyProperties;
  queueFamilyProperties.resize(queueFamilyPropertiesCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           queueFamilyProperties.data());
  uint32_t i = 0;
  for (const auto &queueFamilyProperty : queueFamilyProperties) {
    if (queueFamilyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT) {
      queueFamilyIndices.transferFamilyIndice = i;
      queueFamilyIndices.transferFamilyQueueCount =
          queueFamilyProperty.queueCount;
    }
    if (queueFamilyIndices.isComplete()) {
      break;
    }
    i++;
  }
  return queueFamilyIndices;
}

GraphicsPresentQueueFramilyIndices
findGraphicsPresentQueueFamilyIndices(VkPhysicalDevice device,
                                      VkSurfaceKHR *surface) {
  uint32_t queueFamilyPropertiesCount;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           nullptr);
  if (queueFamilyPropertiesCount <= 0) {
    std::runtime_error("Failed to get Queue Families from Physcial Device");
  }

  GraphicsPresentQueueFramilyIndices queueFamilyIndices;

  std::vector<VkQueueFamilyProperties> queueFamilyProperties;
  queueFamilyProperties.resize(queueFamilyPropertiesCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           queueFamilyProperties.data());
  uint32_t i = 0;
  for (const auto &queueFamilyProperty : queueFamilyProperties) {
    VkBool32 presentable;
    if (queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      queueFamilyIndices.graphicsFamilyIndice = i;
      queueFamilyIndices.graphicsFamilyQueueCount =
          queueFamilyProperty.queueCount;
    }
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *surface, &presentable);
    if (presentable == true) {
      queueFamilyIndices.presentFamilyIndice = i;
      queueFamilyIndices.presentFamilyQueueCount =
          queueFamilyProperty.queueCount;
    }

    if (queueFamilyIndices.isComplete()) {
      break;
    }
    i++;
  }
  return queueFamilyIndices;
}

std::optional<uint32_t> QueueFamilyIndices::getFamilyIndice(VkQueueFlags flag) {
  std::optional<uint32_t> data;
  uint32_t i = 0;
  for (const auto &f : familyFlags) {
    if (f == flag) {
      break;
    }
    i++;
  }
  if (i < familyFlags.size()) {
    data = familyIndices.at(i);
  }
  return data;
}
void QueueFamilyIndices::setQueueFlags(std::vector<VkQueueFlags> flags) {
  for (const auto &flag : flags) {
    familyFlags.push_back(flag);
    familyIndices.push_back(std::optional<uint32_t>());
    queueCounts.push_back(0);
  }
  presentable = false;
};

VkBool32 QueueFamilyIndices::isComplete() {
  for (const auto &index : familyIndices) {
    if (index.has_value() == false) {
      return false;
    }
  }

  if (presentable && presentFamily.has_value() == false) {
    return false;
  }

  return true;
};

QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice device,
                                          std::vector<VkQueueFlags> queueFlags,
                                          VkSurfaceKHR *surface) {

  uint32_t queueFamilyPropertiesCount;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           nullptr);
  if (queueFamilyPropertiesCount <= 0) {
    std::runtime_error("Failed to get Queue Families from Physcial Device");
  }

  QueueFamilyIndices indices;
  indices.setQueueFlags(queueFlags);

  std::vector<VkQueueFamilyProperties> queueFamilyProperties;
  queueFamilyProperties.resize(queueFamilyPropertiesCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           queueFamilyProperties.data());

  uint32_t f = 0;
  std::vector<uint32_t> selectedFamilies;
  for (auto &flag : indices.familyFlags) {
    uint32_t i = 0;
    for (const auto &queueFamilyProperty : queueFamilyProperties) {
      bool continueShearch = false;
      for (const auto &family : selectedFamilies) {
        if (family == i) {
          continueShearch = true;
          break;
        }
      }
      if (continueShearch == true) {
        i++;
        continue;
      }
      if (surface != nullptr && indices.presentable == false) {
        VkSurfaceKHR tempSurface = *surface;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, tempSurface,
                                             &indices.presentable);
        if (indices.presentable) {
          indices.presentFamily = i;
        }
      }
      if (queueFamilyProperty.queueFlags & flag) {
        indices.familyIndices.at(f) = i;
        indices.queueCounts.at(f) = queueFamilyProperty.queueCount;
        selectedFamilies.push_back(i);
        break;
      }
      i++;
    }
    if (indices.isComplete()) {
      break;
    }
    f++;
  }

  return indices;
};
