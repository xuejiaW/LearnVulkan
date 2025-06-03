#pragma once
#include "CommandBuffers/CommandBuffersMgr.h"

class DepthBufferMgr
{
public:
    static void createDepthResources();
    static void destroyDepthResources();
    static VkFormat findDepthFormat();
    static VkImage depthImage;
    static VkDeviceMemory depthImageMemory;
    static VkImageView depthImageView;

private:
    static VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};
