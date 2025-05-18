#pragma once
#include <vulkan/vulkan_core.h>

class LogicalDevicesMgr
{
public:
    void static createLogicalDevice();
    void static destroyLogicalDevice();
    static VkDevice device;
    static VkQueue graphicsQueue;
    static VkQueue presentQueue;
};




















