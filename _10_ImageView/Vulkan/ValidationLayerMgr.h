#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>

class ValidationLayerMgr
{
public:
    static void initialize();
    static void populateInstanceCreateInfo(VkInstanceCreateInfo& createInfo, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo);
    static bool checkValidationLayerSupport();
    static bool enableValidationLayers;
    static std::vector<const char*> validationLayers;
};






