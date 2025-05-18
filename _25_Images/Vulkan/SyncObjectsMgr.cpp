#include "SyncObjectsMgr.h"

#include <stdexcept>

#include "LogicalDevicesMgr.h"
#include "GraphicPipeline/GraphicsPipelineMgr.h"

std::vector<VkSemaphore> SyncObjectsMgr::imageAvailableSemaphores{};
std::vector<VkSemaphore> SyncObjectsMgr::renderFinishedSemaphores{};
std::vector<VkFence> SyncObjectsMgr::inFlightFences{};

void SyncObjectsMgr::createSyncObjects()
{
    imageAvailableSemaphores.resize(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < imageAvailableSemaphores.size(); i++)
    {
        if (vkCreateSemaphore(LogicalDevicesMgr::device, &semaphoreCreateInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(LogicalDevicesMgr::device, &semaphoreCreateInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(LogicalDevicesMgr::device, &fenceCreateInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create sync objects for a frame");
        }
    }
}

void SyncObjectsMgr::destroySyncObjects()
{
    for (size_t i = 0; i < imageAvailableSemaphores.size(); i++)
    {
        vkDestroySemaphore(LogicalDevicesMgr::device, imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(LogicalDevicesMgr::device, renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(LogicalDevicesMgr::device, inFlightFences[i], nullptr);
    }
}






