#pragma once

#include "SwapChainSupportDetails.h"
#include <vulkan/vulkan_core.h>

class SwapChainMgr
{
public:
    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    static void createSwapChain();
    static void createImageViews();
    static void destroySwapChain();
    static void destroyImageViews();
    static VkSwapchainKHR swapChain;
    static std::vector<VkImage> images;
    static std::vector<VkImageView> imageViews;
    static VkFormat imageFormat;
    static VkExtent2D imageExtent;

private:
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};

