#pragma once
#include <vulkan/vulkan_core.h>

class TextureMgr
{
public:
    static void createTextureImage();
    static void destroyTextureImage();
    static VkImage textureImage;
    static VkDeviceMemory textureImageMemory;
    static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    static void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                            VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
};
