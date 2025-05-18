#include "VertexDataMgr.h"

#include <stdexcept>

#include "../LogicalDevicesMgr.h"
#include "../PhysicalDevicesMgr.h"
#include "../Utils/BufferHelper.h"

std::vector<Vertex> VertexDataMgr::vertices =
    {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

std::vector<uint16_t> VertexDataMgr::indices =
    {
        0, 1, 2,
        2, 3, 0
    };

VkBuffer VertexDataMgr::vertexBuffer = VK_NULL_HANDLE;
VkDeviceMemory VertexDataMgr::vertexBufferMemory = VK_NULL_HANDLE;

VkBuffer VertexDataMgr::indexBuffer = VK_NULL_HANDLE;
VkDeviceMemory VertexDataMgr::indexBufferMemory = VK_NULL_HANDLE;

void VertexDataMgr::createVertexBuffer()
{
    const VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;
    BufferHelper::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               stagingBuffer, stagingBufferMemory);

    void* mappedMemory;
    vkMapMemory(LogicalDevicesMgr::device, stagingBufferMemory, 0, bufferSize, 0, &mappedMemory);
    memcpy(mappedMemory, vertices.data(), bufferSize);
    vkUnmapMemory(LogicalDevicesMgr::device, stagingBufferMemory);

    BufferHelper::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT,
                               vertexBuffer, vertexBufferMemory);

    BufferHelper::copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(LogicalDevicesMgr::device, stagingBuffer, nullptr);
    vkFreeMemory(LogicalDevicesMgr::device, stagingBufferMemory, nullptr);
}

void VertexDataMgr::createIndexBuffer()
{
    const VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;
    BufferHelper::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               stagingBuffer, stagingBufferMemory);

    void* mappedMemory;
    vkMapMemory(LogicalDevicesMgr::device, stagingBufferMemory, 0, bufferSize, 0, &mappedMemory);
    memcpy(mappedMemory, indices.data(), bufferSize);
    vkUnmapMemory(LogicalDevicesMgr::device, stagingBufferMemory);

    BufferHelper::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT,
                               indexBuffer, indexBufferMemory);

    BufferHelper::copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(LogicalDevicesMgr::device, stagingBuffer, nullptr);
    vkFreeMemory(LogicalDevicesMgr::device, stagingBufferMemory, nullptr);
}

void VertexDataMgr::destroyIndexBuffer()
{
    vkDestroyBuffer(LogicalDevicesMgr::device, indexBuffer, nullptr);
    vkFreeMemory(LogicalDevicesMgr::device, indexBufferMemory, nullptr);
}


void VertexDataMgr::destroyVertexBuffer()
{
    vkDestroyBuffer(LogicalDevicesMgr::device, vertexBuffer, nullptr);
    vkFreeMemory(LogicalDevicesMgr::device, vertexBufferMemory, nullptr);
}


