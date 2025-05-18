#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>

class PhysicalDevicesMgr
{
public:
    void static pickPhysicalDevice(VkInstance instance);
    bool static checkDeviceExtensionsSupport(VkPhysicalDevice device);
    static VkPhysicalDevice physicalDevice;
    static std::vector<const char*> deviceExtensions;
private:
    static bool isDeviceSuitable(VkPhysicalDevice device);
};





















