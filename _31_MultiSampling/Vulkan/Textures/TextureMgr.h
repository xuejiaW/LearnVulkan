#pragma once
#include <vulkan/vulkan_core.h>

#include <string>

class TextureMgr
{
public:
    static void createTextureImage(std::string path);
    static void destroyTextureImage();

    static void createTextureImageView();
    static void destroyTextureImageView();

    static void createTextureSampler();
    static void destroyTextureSampler();

    static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipmapLevels);
    static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    static void createImage(uint32_t width, uint32_t height, uint32_t mipmapLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                            VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                            VkDeviceMemory& imageMemory);
    static void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

    static uint32_t mipLevels;
    static VkImage textureImage;
    static VkDeviceMemory textureImageMemory;
    static VkImageView textureImageView;
    static VkSampler textureSampler;
};