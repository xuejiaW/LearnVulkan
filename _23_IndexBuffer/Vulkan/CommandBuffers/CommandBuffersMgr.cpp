#include "CommandBuffersMgr.h"

#include <stdexcept>

#include "../LogicalDevicesMgr.h"
#include "../GraphicPipeline/GraphicsPipelineMgr.h"
#include "../QueueFamily/QueueFamilyMgr.h"
#include "../QueueFamily/QueueFamilyIndices.h"

VkCommandPool CommandBuffersMgr::commandPool = VK_NULL_HANDLE;
std::vector<VkCommandBuffer> CommandBuffersMgr::commandBuffers = {};

void CommandBuffersMgr::createCommandPool()
{
    auto indices = QueueFamilyMgr::findQueueFamilies(PhysicalDevicesMgr::physicalDevice);
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(LogicalDevicesMgr::device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
}

void CommandBuffersMgr::destroyCommandPool()
{
    vkDestroyCommandPool(LogicalDevicesMgr::device, commandPool, nullptr);
}

void CommandBuffersMgr::createCommandBuffers()
{
    commandBuffers.resize(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT);
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(LogicalDevicesMgr::device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffer!");
    }
}




