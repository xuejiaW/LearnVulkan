#pragma once
#include <vulkan/vulkan_core.h>

class PhysicalDevicesMgr
{
public:
    void static pickPhysicalDevice(VkInstance instance);
    static VkPhysicalDevice physicalDevice;

private:
    static bool isDeviceSuitable(VkPhysicalDevice device);
};

