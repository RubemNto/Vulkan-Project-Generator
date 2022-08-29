#include "../header/program.hpp"

VkBool32 QueueFamilyIndices::isComplete() {
  for (const auto &index : familyIndices) {
    if (index.has_value() == false) {
      return false;
    }
  }
  return true;
}

void QueueFamilyIndices::setQueueFlags(std::vector<VkQueueFlags> flags) {
  for (const auto &flag : flags) {
    familyFlags.push_back(flag);
    familyIndices.push_back(std::optional<uint32_t>());
  }
}

QueueFamilyIndices
findQueueFamilyIndices(VkPhysicalDevice device,
                       std::vector<VkQueueFlags> queueFlags) {

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
  for (auto &flag : indices.familyFlags) {
    uint32_t i = 0;
    for (const auto &queueFamilyProperty : queueFamilyProperties) {
      if (queueFamilyProperty.queueFlags & flag) {
        indices.familyIndices.at(f) = i;
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
}

void Program::run() {
  createVulkanInstance(VK_MAKE_VERSION(1, 0, 0));
  pickPhysicalDevice();
  cleanup();
}

void Program::createVulkanInstance(uint32_t minAPIVersion) {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.apiVersion = minAPIVersion;
  appInfo.applicationVersion = 1;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  if (vkCreateInstance(&createInfo, nullptr, &pInstance) != VK_SUCCESS) {
    std::runtime_error("Failed to create Vulkan Instance!");
  } else {
    std::cout << "Created Vulkan Instance!" << std::endl;
    pApiVersion = minAPIVersion;
  }
}

VkBool32
Program::checkDeviceSuitability(VkPhysicalDevice device, uint32_t minAPIVersion,
                                VkPhysicalDeviceType deviceType,
                                std::vector<VkQueueFlags> deviceQueueFlags) {
  // get device Properties
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(device, &properties);

  if ((properties.apiVersion < minAPIVersion) ||
      (properties.deviceType != deviceType)) {
    return VK_FALSE;
  }

  // get device Features
  // VkPhysicalDeviceFeatures features;
  // vkGetPhysicalDeviceFeatures(device, &features);

  // get device Queue Families
  std::vector<VkQueueFlags> queueFlags = {
      VK_QUEUE_GRAPHICS_BIT,
  };

  QueueFamilyIndices queueFamilyIndices =
      findQueueFamilyIndices(device, queueFlags);
  if (queueFamilyIndices.isComplete() != VK_TRUE) {
    return VK_FALSE;
  }
  return VK_TRUE;
}

void Program::pickPhysicalDevice() {
  uint32_t physicalDeviceCount;
  vkEnumeratePhysicalDevices(pInstance, &physicalDeviceCount, nullptr);

  if (physicalDeviceCount <= 0) {
    std::runtime_error("No Vulkan Supported Physical Device Found");
  }

  std::vector<VkPhysicalDevice> physicalDevices;
  physicalDevices.resize(physicalDeviceCount);
  vkEnumeratePhysicalDevices(pInstance, &physicalDeviceCount,
                             physicalDevices.data());

  std::vector<VkQueueFlags> deviceQueueFlags = {VK_QUEUE_GRAPHICS_BIT,
                                                VK_QUEUE_TRANSFER_BIT};

  for (const auto &device : physicalDevices) {
    if (checkDeviceSuitability(device, pApiVersion,
                               VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
                               deviceQueueFlags) == VK_TRUE) {
      pPhysialDevice = device;
      std::cout << "Selected Vulkan Supported Physical Device!" << std::endl;
      return;
    }
  }
  std::runtime_error("Failed to select suitable Physcial Device");
}

void Program::cleanup() { vkDestroyInstance(pInstance, nullptr); }
