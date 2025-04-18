#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>

class ExtensionsMgr
{
public:
    static void checkAvailableExtensions(const VkInstanceCreateInfo& createInfo);
    static void checkRequiredGlfwExtensions();
    static std::vector<const char*> getRequiredExtensions();
};












