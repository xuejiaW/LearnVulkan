#include "PhysicalDevicesMgr.h"

#include <set>
#include <stdexcept>
#include <vector>

#include "QueueFamily/QueueFamilyIndices.h"
#include "QueueFamily/QueueFamilyMgr.h"
#include "SwapChain/SwapChainMgr.h"
#include "SwapChain/SwapChainSupportDetails.h"

VkPhysicalDevice PhysicalDevicesMgr::physicalDevice = VK_NULL_HANDLE;
std::vector<const char*> PhysicalDevicesMgr::deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
VkSampleCountFlagBits PhysicalDevicesMgr::msaaSamples = VK_SAMPLE_COUNT_1_BIT;

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
            msaaSamples = getMaxUsableSampleCount();
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("failed to find a suitable GPU!");
}

bool PhysicalDevicesMgr::checkDeviceExtensionsSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions)
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
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

    bool extensionSupport = checkDeviceExtensionsSupport(device);
    bool swapChainAdequate = false;
    if (extensionSupport)
    {
        SwapChainSupportDetails swapChainSupport = SwapChainMgr::querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return deviceSuitable && queueFamilySuitable && extensionSupport && swapChainAdequate && deviceFeatures.samplerAnisotropy;
}

VkSampleCountFlagBits PhysicalDevicesMgr::getMaxUsableSampleCount()
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    VkSampleCountFlags counts = deviceProperties.limits.framebufferColorSampleCounts &
                                deviceProperties.limits.framebufferDepthSampleCounts;

    if (counts & VK_SAMPLE_COUNT_64_BIT) return VK_SAMPLE_COUNT_64_BIT;
    if (counts & VK_SAMPLE_COUNT_32_BIT) return VK_SAMPLE_COUNT_32_BIT;
    if (counts & VK_SAMPLE_COUNT_16_BIT) return VK_SAMPLE_COUNT_16_BIT;
    if (counts & VK_SAMPLE_COUNT_8_BIT) return VK_SAMPLE_COUNT_8_BIT;
    if (counts & VK_SAMPLE_COUNT_4_BIT) return VK_SAMPLE_COUNT_4_BIT;
    if (counts & VK_SAMPLE_COUNT_2_BIT) return VK_SAMPLE_COUNT_2_BIT;

    return VK_SAMPLE_COUNT_1_BIT;
}