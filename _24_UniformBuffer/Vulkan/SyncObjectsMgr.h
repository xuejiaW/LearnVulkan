#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>

class SyncObjectsMgr
{
public:
    static void createSyncObjects();
    static void destroySyncObjects(); 
    static std::vector<VkSemaphore> imageAvailableSemaphores;
    static std::vector<VkSemaphore> renderFinishedSemaphores;
    static std::vector<VkFence> inFlightFences;
};






