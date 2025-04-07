#include "LogicDevicesMgr.h"

#include <stdexcept>

#include "ValidationLayerMgr.h"
#include "QueueFamily/QueueFamilyIndices.h"
#include "QueueFamily/QueueFamilyMgr.h"

VkDevice LogicDevicesMgr::device = VK_NULL_HANDLE;
VkQueue LogicDevicesMgr::graphicsQueue = VK_NULL_HANDLE;

void LogicDevicesMgr::createLogicalDevice()
{
    constexpr float queuePriority = 1.0f;
    const QueueFamilyIndices indices = QueueFamilyMgr::findQueueFamilies(PhysicalDevicesMgr::physicalDevice);
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    constexpr VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    if (vkCreateDevice(PhysicalDevicesMgr::physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

void LogicDevicesMgr::destroyLogicalDevice()
{
    vkDestroyDevice(device, nullptr);
}
