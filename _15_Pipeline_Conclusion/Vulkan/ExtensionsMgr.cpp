#include "ExtensionsMgr.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "ValidationLayerMgr.h"

void ExtensionsMgr::checkAvailableExtensions(const VkInstanceCreateInfo& createInfo)
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "available extensions:\n";
    for (const auto& extension : extensions)
    {
        std::cout << '\t' << extension.extensionName << '\n';
    }
}

void ExtensionsMgr::checkRequiredGlfwExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtension = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::cout << "required extensions: \n";
    for (uint32_t i = 0; i != glfwExtensionCount; ++i)
    {
        std::cout << "\t" << glfwExtension[i] << '\n';
    }
}

std::vector<const char*> ExtensionsMgr::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // Use glfwExtensions to initialize the vector
    std::vector<const char*> extensions{glfwExtensions, glfwExtensions + glfwExtensionCount};
    if (ValidationLayerMgr::enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}











