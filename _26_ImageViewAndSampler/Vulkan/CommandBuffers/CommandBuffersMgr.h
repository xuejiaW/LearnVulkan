#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>

class CommandBuffersMgr
{
public:
    static void createCommandPool();
    static void destroyCommandPool();
    static VkCommandPool commandPool;

    static VkCommandBuffer beginSingleTimeCommands();
    static void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    static void createCommandBuffers();
    static std::vector<VkCommandBuffer> commandBuffers;
};
