#include "VertexDataMgr.h"

#include <stdexcept>

#include "../LogicalDevicesMgr.h"
#include "../PhysicalDevicesMgr.h"

std::vector<Vertex> VertexDataMgr::vertices =
    {
        {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

VkBuffer VertexDataMgr::vertexBuffer = VK_NULL_HANDLE;
VkDeviceMemory VertexDataMgr::vertexBufferMemory = VK_NULL_HANDLE;

void VertexDataMgr::createVertexBuffer()
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(Vertex) * vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(LogicalDevicesMgr::device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(LogicalDevicesMgr::device, vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findSuitableMemoryType(memRequirements.memoryTypeBits,
                                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(LogicalDevicesMgr::device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate vertex buffer memory");
    }

    vkBindBufferMemory(LogicalDevicesMgr::device, vertexBuffer, vertexBufferMemory, 0);
    void* mappedMemory;
    vkMapMemory(LogicalDevicesMgr::device, vertexBufferMemory, 0, bufferInfo.size, 0, &mappedMemory);
    memcpy(mappedMemory, vertices.data(), bufferInfo.size);
    vkUnmapMemory(LogicalDevicesMgr::device, vertexBufferMemory);
}

uint32_t VertexDataMgr::findSuitableMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties physicalMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(PhysicalDevicesMgr::physicalDevice, &physicalMemoryProperties);
    for (uint32_t i = 0; i < physicalMemoryProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (physicalMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type");
}


void VertexDataMgr::destroyVertexBuffer()
{
    vkDestroyBuffer(LogicalDevicesMgr::device, vertexBuffer, nullptr);
    vkFreeMemory(LogicalDevicesMgr::device, vertexBufferMemory, nullptr);
}