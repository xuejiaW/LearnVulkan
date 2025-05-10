#pragma once
#include <vector>

#include "Vertex.h"

class VertexDataMgr
{
public:
    static void createVertexBuffer();
    static void destroyVertexBuffer();
    static VkBuffer vertexBuffer;
    static VkDeviceMemory vertexBufferMemory;

private:
    static std::vector<Vertex> vertices;
    static uint32_t findSuitableMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};