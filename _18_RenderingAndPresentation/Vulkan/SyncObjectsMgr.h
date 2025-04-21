#pragma once
#include <vulkan/vulkan_core.h>

class SyncObjectsMgr
{
public:
    static void createSyncObjects();
    static void destroySyncObjects(); 
    static VkSemaphore imageAvailableSemaphore;
    static VkSemaphore renderFinishedSemaphore;
    static VkFence inFlightFence;
};