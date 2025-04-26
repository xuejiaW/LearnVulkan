// ReSharper disable CppUnreachableCode
// ReSharper disable CppRedundantBooleanExpressionArgument
#include "HelloTriangleApplication.h"

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

#include "Vulkan/DebugMessengerMgr.h"
#include "Vulkan/ExtensionsMgr.h"
#include "Vulkan/LogicalDevicesMgr.h"
#include "Vulkan/PhysicalDevicesMgr.h"
#include "Vulkan/ValidationLayerMgr.h"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

void HelloTriangleApplication::initVulkan()
{
    createInstance();
    DebugMessengerMgr::setupDebugMessenger(instance);
    PhysicalDevicesMgr::pickPhysicalDevice(instance);
    LogicalDevicesMgr::createLogicalDevice();
}

void HelloTriangleApplication::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}

void HelloTriangleApplication::cleanup()
{
    LogicalDevicesMgr::destroyLogicalDevice();
    if (ValidationLayerMgr::enableValidationLayers)
        DebugMessengerMgr::destroyDebugUtilsMessengerExt(instance, nullptr);

    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void HelloTriangleApplication::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "07_Vulkan_Window", nullptr, nullptr);
}

void HelloTriangleApplication::createInstance()
{
    if (ValidationLayerMgr::enableValidationLayers && !ValidationLayerMgr::checkValidationLayerSupport())
    {
        throw std::runtime_error("Validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello_Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    const auto extensions = ExtensionsMgr::getRequiredExtensions();

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Declare debugCreateInfo as it need to remain in scope until the vkCreateInstance call
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    ValidationLayerMgr::populateInstanceCreateInfo(createInfo, debugCreateInfo);

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance!");
    }

    ExtensionsMgr::checkAvailableExtensions(createInfo);
    ExtensionsMgr::checkRequiredGlfwExtensions();
}


void HelloTriangleApplication::run()
{
    ValidationLayerMgr::initialize();
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}
