#include "PhysicalDevicesMgr.h"

#include <stdexcept>
#include <vector>

#include "QueueFamily/QueueFamilyIndices.h"
#include "QueueFamily/QueueFamilyMgr.h"

VkPhysicalDevice PhysicalDevicesMgr::physicalDevice = VK_NULL_HANDLE;


void PhysicalDevicesMgr::pickPhysicalDevice(VkInstance instance)
{
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0)
        throw std::runtime_error("failed to find GPUs with Vulkan support!");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices)
    {
        if (isDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("failed to find a suitable GPU!");
}

bool PhysicalDevicesMgr::isDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    QueueFamilyIndices indices = QueueFamilyMgr::findQueueFamilies(device);
    const bool deviceSuitable = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
                                deviceFeatures.geometryShader;
    const bool queueFamilySuitable = indices.isComplete();

    return deviceSuitable && queueFamilySuitable;
}
