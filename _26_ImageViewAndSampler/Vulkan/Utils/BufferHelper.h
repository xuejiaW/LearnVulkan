#pragma once
#include <vulkan/vulkan_core.h>

class BufferHelper
{
public:
    static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
                             VkDeviceMemory& bufferMemory);
    static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    static uint32_t findSuitableMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};


