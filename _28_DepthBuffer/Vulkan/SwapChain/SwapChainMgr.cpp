#include "SwapChainMgr.h"

#include <iostream>
#include <stdexcept>

#include "../DepthBufferMgr.h"
#include "../FrameBuffersMgr.h"
#include "../LogicalDevicesMgr.h"
#include "../PhysicalDevicesMgr.h"
#include "../SurfaceMgr.h"
#include "../QueueFamily/QueueFamilyIndices.h"
#include "../QueueFamily/QueueFamilyMgr.h"
#include "../../HelloTriangleApplication.h"
#include "../Utils/ImageHelper.h"

VkSwapchainKHR SwapChainMgr::swapChain{};
std::vector<VkImage> SwapChainMgr::images = {};
std::vector<VkImageView> SwapChainMgr::imageViews = {};
VkFormat SwapChainMgr::imageFormat = VK_FORMAT_UNDEFINED;
VkExtent2D SwapChainMgr::imageExtent = {0, 0};

SwapChainSupportDetails SwapChainMgr::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, SurfaceMgr::surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, SurfaceMgr::surface, &formatCount, nullptr);
    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, SurfaceMgr::surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, SurfaceMgr::surface, &presentModeCount, nullptr);
    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, SurfaceMgr::surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}


VkSurfaceFormatKHR SwapChainMgr::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormat;
    }

    return availableFormats[0];
}

VkPresentModeKHR SwapChainMgr::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return availablePresentMode;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChainMgr::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        return capabilities.currentExtent;

    int width, height;
    glfwGetFramebufferSize(HelloTriangleApplication::window, &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
}

void SwapChainMgr::createSwapChain()
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(PhysicalDevicesMgr::physicalDevice);
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        imageCount = swapChainSupport.capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = SurfaceMgr::surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = QueueFamilyMgr::findQueueFamilies(PhysicalDevicesMgr::physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    if (indices.graphicsFamily == indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(LogicalDevicesMgr::device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(LogicalDevicesMgr::device, swapChain, &imageCount, nullptr);
    images.resize(imageCount);
    vkGetSwapchainImagesKHR(LogicalDevicesMgr::device, swapChain, &imageCount, images.data());
    imageFormat = surfaceFormat.format;
    imageExtent = extent;
}

void SwapChainMgr::recreateSwapChain()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(HelloTriangleApplication::window, &width, &height);
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(HelloTriangleApplication::window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(LogicalDevicesMgr::device);

    FrameBuffersMgr::destroyFramebuffers();
    destroyImageViews();
    destroySwapChain();
    DepthBufferMgr::destroyDepthResources();

    createSwapChain();
    createImageViews();
    DepthBufferMgr::createDepthResources();
    FrameBuffersMgr::createFramebuffers();
}

void SwapChainMgr::createImageViews()
{
    imageViews.resize(images.size());

    for (size_t i = 0; i < images.size(); i++)
    {
        imageViews[i] = ImageHelper::createImageView(images[i], imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

void SwapChainMgr::destroySwapChain()
{
    vkDestroySwapchainKHR(LogicalDevicesMgr::device, swapChain, nullptr);
}

void SwapChainMgr::destroyImageViews()
{
    for (VkImageView imageView : imageViews)
    {
        vkDestroyImageView(LogicalDevicesMgr::device, imageView, nullptr);
    }
}