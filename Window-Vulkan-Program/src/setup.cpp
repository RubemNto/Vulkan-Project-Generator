#include "../header/setup.hpp"

VkBool32 checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
      if (std::strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

void Setup::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(WIDTH, HEIGHT, "Window Vulkan Program", nullptr,
                            nullptr);
}

void Setup::createVulkanInstance(uint32_t minAPIVersion) {
  if (enableValidationLayers && !checkValidationLayerSupport()) {
    throw std::runtime_error("validation layers requested, but not available!");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.apiVersion = minAPIVersion;
  appInfo.applicationVersion = 1;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;

  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, &pInstance) != VK_SUCCESS) {
    std::runtime_error("Failed to create Vulkan Instance!");
  } else {
    std::cout << "Created Vulkan Instance!" << std::endl;
    pApiVersion = minAPIVersion;
  }
}

VkBool32 Setup::checkDeviceExtensionSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(deviceExtensions.begin(),
                                           deviceExtensions.end());

  for (const auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

VkBool32
Setup::checkDeviceSuitability(VkPhysicalDevice device, uint32_t minAPIVersion,
                              VkPhysicalDeviceType deviceType,
                              std::vector<VkQueueFlags> deviceQueueFlags,
                              VkSurfaceKHR surface) {
  // get device Properties
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(device, &properties);

  if ((properties.apiVersion < minAPIVersion) ||
      (properties.deviceType != deviceType)) {
    return VK_FALSE;
  }

  // get device Features
  VkPhysicalDeviceFeatures features;
  vkGetPhysicalDeviceFeatures(device, &features);
  if (features.samplerAnisotropy == false) {
    return VK_FALSE;
  }

  // get device Queue Families
  QueueFamilyIndices queueFamilyIndices =
      findQueueFamilyIndices(device, deviceQueueFlags, nullptr);
  if (queueFamilyIndices.isComplete() != VK_TRUE) {
    return VK_FALSE;
  }

  if (checkDeviceExtensionSupport(device) == VK_FALSE) {
    return VK_FALSE;
  } else {
    SwapChainSupportDetails swapChainSupport =
        querySwapChainSupport(device, surface);
    bool swapChainAdequate = !swapChainSupport.formats.empty() &&
                             !swapChainSupport.presentModes.empty();
    if (swapChainAdequate == false) {
      return VK_FALSE;
    }

    return VK_TRUE;
  }
}

void Setup::pickPhysicalDevice(VkPhysicalDeviceType type,
                               std::vector<VkQueueFlags> deviceQueueFlags,
                               VkSurfaceKHR surface) {
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
    if (checkDeviceSuitability(device, pApiVersion, type, deviceQueueFlags,
                               surface) == VK_TRUE) {
      pPhysialDevice = device;
      std::cout << "Selected Vulkan Supported Physical Device!" << std::endl;
      return;
    }
  }
  std::runtime_error("Failed to select suitable Physcial Device");
}

void Setup::createLogicalDevice(VkSurfaceKHR *surface) {
  QueueFamilyIndices indices =
      findQueueFamilyIndices(pPhysialDevice, deviceQueueFlags, surface);
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

  uint32_t familyCout = static_cast<uint32_t>(indices.familyIndices.size());
  float queuePriority = 1.0f;
  for (uint32_t i = 0; i < familyCout; i++) {
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueCount = indices.queueCounts.at(i);
    queueInfo.queueFamilyIndex = indices.familyIndices.at(i).value();
    queueInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueInfo);
  }

  if (indices.presentable == true &&
      indices.presentFamily.has_value() == true) {
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueCount = 1;
    queueInfo.queueFamilyIndex = indices.presentFamily.value();
    queueInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueInfo);
  }
  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount =
      static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions.data();
  createInfo.pEnabledFeatures = &deviceFeatures;
  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }
  if (vkCreateDevice(pPhysialDevice, &createInfo, nullptr, &pDevice) !=
      VK_SUCCESS) {
    std::runtime_error("Failed to create Logical Device");
  }

  std::cout << "Created Vulkan Logical Device!" << std::endl;
  uint32_t i = 0;
  for (const auto &queue : indices.familyIndices) {
    VkQueue deviceQueue;
    vkGetDeviceQueue(pDevice, queue.value(), 0, &deviceQueue);
    pDeviceQueues.push_back(deviceQueue);
    i++;
  }
  if (indices.presentable) {
    VkQueue deviceQueue;
    vkGetDeviceQueue(pDevice, indices.presentFamily.value(), 0, &deviceQueue);
    pDeviceQueues.push_back(deviceQueue);
  }
}
