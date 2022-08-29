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
    queueCounts.push_back(0);
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
        indices.queueCounts.at(f) = queueFamilyProperty.queueCount;
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
  pickPhysicalDevice(type, deviceQueueFlags);
  createLogicalDevice();
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
  QueueFamilyIndices queueFamilyIndices =
      findQueueFamilyIndices(device, deviceQueueFlags);
  if (queueFamilyIndices.isComplete() != VK_TRUE) {
    return VK_FALSE;
  }
  return VK_TRUE;
}

void Program::pickPhysicalDevice(VkPhysicalDeviceType type,
                                 std::vector<VkQueueFlags> deviceQueueFlags) {
  uint32_t physicalDeviceCount;
  vkEnumeratePhysicalDevices(pInstance, &physicalDeviceCount, nullptr);

  if (physicalDeviceCount <= 0) {
    std::runtime_error("No Vulkan Supported Physical Device Found");
  }

  std::vector<VkPhysicalDevice> physicalDevices;
  physicalDevices.resize(physicalDeviceCount);
  vkEnumeratePhysicalDevices(pInstance, &physicalDeviceCount,
                             physicalDevices.data());

  for (const auto &device : physicalDevices) {
    if (checkDeviceSuitability(device, pApiVersion, type, deviceQueueFlags) ==
        VK_TRUE) {
      pPhysialDevice = device;
      std::cout << "Selected Vulkan Supported Physical Device!" << std::endl;
      return;
    }
  }
  std::runtime_error("Failed to select suitable Physcial Device");
}

void Program::createLogicalDevice() {
  QueueFamilyIndices indices =
      findQueueFamilyIndices(pPhysialDevice, deviceQueueFlags);
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  for (uint32_t i = 0; i < static_cast<uint32_t>(deviceQueueFlags.size());
       i++) {
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueCount = indices.queueCounts.at(i);
    queueInfo.queueFamilyIndex = indices.familyIndices.at(i).value();
    queueCreateInfos.push_back(queueInfo);
  }

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount =
      static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  if (vkCreateDevice(pPhysialDevice, &createInfo, nullptr, &pDevice) !=
      VK_SUCCESS) {
    std::runtime_error("Failed to create Logical Device");
  }
  std::cout << "Created Vulkan Logical Device!" << std::endl;
}

void Program::cleanup() {
  vkDestroyInstance(pInstance, nullptr);
  vkDestroyDevice(pDevice, nullptr);
}
