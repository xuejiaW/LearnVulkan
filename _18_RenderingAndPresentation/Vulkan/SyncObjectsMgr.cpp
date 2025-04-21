#include "SyncObjectsMgr.h"

#include <stdexcept>

#include "LogicDevicesMgr.h"

VkSemaphore SyncObjectsMgr::imageAvailableSemaphore{};
VkSemaphore SyncObjectsMgr::renderFinishedSemaphore{};
VkFence SyncObjectsMgr::inFlightFence{};

void SyncObjectsMgr::createSyncObjects()
{
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateSemaphore(LogicDevicesMgr::device, &semaphoreCreateInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(LogicDevicesMgr::device, &semaphoreCreateInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(LogicDevicesMgr::device, &fenceCreateInfo, nullptr, &inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create sync objects for a frame");
    }
}

void SyncObjectsMgr::destroySyncObjects()
{
    vkDestroySemaphore(LogicDevicesMgr::device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(LogicDevicesMgr::device, renderFinishedSemaphore, nullptr);
    vkDestroyFence(LogicDevicesMgr::device, inFlightFence, nullptr);
}