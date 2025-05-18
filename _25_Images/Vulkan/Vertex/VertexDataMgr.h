#pragma once
#include <vector>
#include "Vertex.h"

class VertexDataMgr
{
public:
    static std::vector<Vertex> vertices;
    static std::vector<uint16_t> indices;
    static void createVertexBuffer();
    static void createIndexBuffer();
    static void destroyVertexBuffer();
    static void destroyIndexBuffer();
    
    static VkBuffer vertexBuffer;
    static VkDeviceMemory vertexBufferMemory;

    static VkBuffer indexBuffer;
    static VkDeviceMemory indexBufferMemory;

private:
    static uint32_t findSuitableMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

