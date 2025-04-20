#pragma once
#include <vulkan/vulkan_core.h>

class CommandBuffersMgr
{
public:
    static void createCommandPool();
    static void destroyCommandPool();
    static VkCommandPool commandPool;

    static void createCommandBuffer();
    static VkCommandBuffer commandBuffer;
};