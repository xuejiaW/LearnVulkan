#pragma once
#include <vector>

#include "Vertex.h"

class VertexDataMgr
{
public:
    static std::vector<Vertex> vertices;
    static void createVertexBuffer();
    static void destroyVertexBuffer();
    static VkBuffer vertexBuffer;
    static VkDeviceMemory vertexBufferMemory;

private:
    static uint32_t findSuitableMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
                             VkDeviceMemory& bufferMemory);
    static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};