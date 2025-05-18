#pragma once
#include <vulkan/vulkan_core.h>

class DebugMessengerMgr
{
public:
    static void setupDebugMessenger(VkInstance instance);
    static void destroyDebugUtilsMessengerExt(VkInstance instance, const VkAllocationCallbacks* pAllocator);
    static VkResult createDebugUtilsMessengerExt(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                 const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

private:
    static VkDebugUtilsMessengerEXT debugMessenger;
};
























