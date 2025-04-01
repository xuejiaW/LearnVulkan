#pragma once
#include <vulkan/vulkan_core.h>

class ValidationLayerMgr
{
public:
    static void initialize();
    static void populateInstanceCreateInfo(VkInstanceCreateInfo& createInfo, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo);
    static bool checkValidationLayerSupport();
    static bool enableValidationLayers;
};
