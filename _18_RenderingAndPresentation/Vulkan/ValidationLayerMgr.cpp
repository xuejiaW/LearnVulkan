#include "ValidationLayerMgr.h"

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <vector>

#include "DebugMessengerMgr.h"

std::vector<const char*> ValidationLayerMgr::validationLayers = {"VK_LAYER_KHRONOS_validation"};

bool ValidationLayerMgr::enableValidationLayers = false;

void ValidationLayerMgr::initialize()
{
#ifdef NDEBUG
    enableValidationLayers = false;
#else
    enableValidationLayers = true;
#endif
}

void ValidationLayerMgr::populateInstanceCreateInfo(VkInstanceCreateInfo& createInfo, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo)
{
    if (enableValidationLayers)
    {
        DebugMessengerMgr::populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        createInfo.pNext = &debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }
}

bool ValidationLayerMgr::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    bool layerFound = false;
    const char* layerName = validationLayers[0];

    for (const auto& layerProperties : availableLayers)
    {
        if (strcmp(layerName, layerProperties.layerName) == 0)
        {
            layerFound = true;
            break;
        }
    }

    if (!layerFound) return false;
    return true;
}











