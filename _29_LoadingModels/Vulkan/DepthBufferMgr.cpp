#include "DepthBufferMgr.h"

#include <stdexcept>

#include "LogicalDevicesMgr.h"
#include "PhysicalDevicesMgr.h"
#include "SwapChain/SwapChainMgr.h"
#include "Textures/TextureMgr.h"
#include "Utils/ImageHelper.h"

VkImage DepthBufferMgr::depthImage = VK_NULL_HANDLE;
VkDeviceMemory DepthBufferMgr::depthImageMemory = VK_NULL_HANDLE;
VkImageView DepthBufferMgr::depthImageView = VK_NULL_HANDLE;

void DepthBufferMgr::createDepthResources()
{
    VkFormat depthFormat = findDepthFormat();
    TextureMgr::createImage(SwapChainMgr::imageExtent.width, SwapChainMgr::imageExtent.height, depthFormat,
                            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    depthImageView = ImageHelper::createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void DepthBufferMgr::destroyDepthResources()
{
    vkDestroyImageView(LogicalDevicesMgr::device, depthImageView, nullptr);
    vkDestroyImage(LogicalDevicesMgr::device, depthImage, nullptr);
    vkFreeMemory(LogicalDevicesMgr::device, depthImageMemory, nullptr);
}


VkFormat DepthBufferMgr::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties properties;
        vkGetPhysicalDeviceFormatProperties(PhysicalDevicesMgr::physicalDevice, format, &properties);
        if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features)
            return format;

        if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features)
            return format;
    }

    throw std::runtime_error("failed to find supporting format!");
}

VkFormat DepthBufferMgr::findDepthFormat()
{
    return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}
