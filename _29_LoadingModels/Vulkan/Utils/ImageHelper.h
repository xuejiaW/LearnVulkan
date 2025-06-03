#pragma once
#include <vulkan/vulkan_core.h>

class ImageHelper
{
public:
    static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
};


