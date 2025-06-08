#include "MsaaMgr.h"

#include "LogicalDevicesMgr.h"
#include "PhysicalDevicesMgr.h"
#include "SwapChain/SwapChainMgr.h"
#include "Textures/TextureMgr.h"
#include "Utils/ImageHelper.h"

VkImage MsaaMgr::colorImage = VK_NULL_HANDLE;
VkImageView MsaaMgr::colorImageView = VK_NULL_HANDLE;
VkDeviceMemory MsaaMgr::colorImageMemory = VK_NULL_HANDLE;

void MsaaMgr::createColorResources()
{
    VkFormat colorFormat = SwapChainMgr::imageFormat;
    TextureMgr::createImage(SwapChainMgr::imageExtent.width, SwapChainMgr::imageExtent.height, 1, PhysicalDevicesMgr::msaaSamples, colorFormat,
                            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);

    colorImageView = ImageHelper::createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}

void MsaaMgr::destroyColorResources()
{
    vkDestroyImageView(LogicalDevicesMgr::device, colorImageView, nullptr);
    vkDestroyImage(LogicalDevicesMgr::device, colorImage, nullptr);
    vkFreeMemory(LogicalDevicesMgr::device, colorImageMemory, nullptr);
}