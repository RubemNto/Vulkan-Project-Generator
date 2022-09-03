#include "../header/queuefamilyindices.hpp"

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
