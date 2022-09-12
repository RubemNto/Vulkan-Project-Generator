#include "../header/presentation.hpp"

void Presentation::createSurface(VkInstance instance, GLFWwindow *window) {
  if (glfwCreateWindowSurface(instance, window, nullptr, &surface) !=
      VK_SUCCESS) {
    std::runtime_error("Failed to create GLFW Window Surface!");
  }
  std::cout << "Created GLFW Window Surface" << std::endl;
}

VkSurfaceFormatKHR
Presentation::pickSurfaceFormat(const std::vector<VkSurfaceFormatKHR> formats) {
  for (const auto &availableFormat : formats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return formats[0];
}

VkPresentModeKHR Presentation::pickPresentMode(
    const std::vector<VkPresentModeKHR> presentModes) {
  for (const auto &availablePresentMode : presentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D
Presentation::pickExtent(GLFWwindow *window,
                         const VkSurfaceCapabilitiesKHR &capabilities) {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                               static_cast<uint32_t>(height)};

    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);

    return actualExtent;
  }
}
// stoped at choosing the right settings for swap chain
void Presentation::createSwapChain(GLFWwindow *window,
                                   std::vector<VkQueueFlags> queueFlags,
                                   VkPhysicalDevice pDevice, VkDevice lDevice) {
  SwapChainSupportDetails swapChainSupport =
      querySwapChainSupport(pDevice, surface);

  VkSurfaceFormatKHR surfaceFormat =
      pickSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode = pickPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = pickExtent(window, swapChainSupport.capabilities);

  swapChainImageFormat = surfaceFormat.format;
  swapChainExtent = extent;

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }
  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices;
  indices.presentable = true;
  indices = findQueueFamilyIndices(pDevice, queueFlags, &surface);
  std::vector<uint32_t> queueFamilyIndices;
  uint32_t i = 0;
  for (const auto &flag : indices.familyFlags) {
    if (flag == VK_QUEUE_GRAPHICS_BIT) {
      queueFamilyIndices.push_back(indices.familyIndices.at(i).value());
      break;
    }
    i++;
  }
  queueFamilyIndices.push_back(indices.presentFamily.value());

  if (queueFamilyIndices.at(0) != queueFamilyIndices.at(1)) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }
  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(lDevice, &createInfo, nullptr, &swapChain) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create swap chain!");
  } else {
    std::cout << "Created Vulkan Swap Chain" << std::endl;
  }
  vkGetSwapchainImagesKHR(lDevice, swapChain, &imageCount, nullptr);
  swapChainImages.reserve(imageCount);
  vkGetSwapchainImagesKHR(lDevice, swapChain, &imageCount,
                          swapChainImages.data());
}
