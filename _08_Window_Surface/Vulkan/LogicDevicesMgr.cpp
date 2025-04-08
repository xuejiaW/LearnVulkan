#include "LogicDevicesMgr.h"

#include <set>
#include <stdexcept>

#include "ValidationLayerMgr.h"
#include "QueueFamily/QueueFamilyIndices.h"
#include "QueueFamily/QueueFamilyMgr.h"

VkDevice LogicDevicesMgr::device = VK_NULL_HANDLE;
VkQueue LogicDevicesMgr::graphicsQueue = VK_NULL_HANDLE;
VkQueue LogicDevicesMgr::presentQueue = VK_NULL_HANDLE;

void LogicDevicesMgr::createLogicalDevice()
{
    constexpr float queuePriority = 1.0f;
    const QueueFamilyIndices indices = QueueFamilyMgr::findQueueFamilies(PhysicalDevicesMgr::physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
    std::set uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {

        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    constexpr VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    if (vkCreateDevice(PhysicalDevicesMgr::physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void LogicDevicesMgr::destroyLogicalDevice()
{
    vkDestroyDevice(device, nullptr);
}
