// ReSharper disable CppUnreachableCode
// ReSharper disable CppRedundantBooleanExpressionArgument
#include "HelloTriangleApplication.h"

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

#include "Vulkan/DebugMessengerMgr.h"
#include "Vulkan/ExtensionsMgr.h"
#include "Vulkan/FrameBuffersMgr.h"
#include "Vulkan/LogicalDevicesMgr.h"
#include "Vulkan/PhysicalDevicesMgr.h"
#include "Vulkan/SurfaceMgr.h"
#include "Vulkan/ValidationLayerMgr.h"
#include "Vulkan/GraphicPipeline/GraphicsPipelineMgr.h"
#include "Vulkan/SwapChain/SwapChainMgr.h"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

GLFWwindow* HelloTriangleApplication::window = nullptr;

void HelloTriangleApplication::run()
{
    ValidationLayerMgr::initialize();
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void HelloTriangleApplication::initVulkan()
{
    createInstance();
    DebugMessengerMgr::setupDebugMessenger(instance);
    SurfaceMgr::createSurface(instance, window);
    PhysicalDevicesMgr::pickPhysicalDevice(instance);
    LogicalDevicesMgr::createLogicalDevice();
    SwapChainMgr::createSwapChain();
    SwapChainMgr::createImageViews();
    GraphicsPipelineMgr::createGraphicsPipeline("Shaders/TriangleVert.spv", "Shaders/TriangleFrag.spv");
    FrameBuffersMgr::createFramebuffers();
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
    FrameBuffersMgr::destroyFramebuffers();
    GraphicsPipelineMgr::destroyGraphicsPipeline();
    SwapChainMgr::destroyImageViews();
    SwapChainMgr::destroySwapChain();
    LogicalDevicesMgr::destroyLogicalDevice();
    if (ValidationLayerMgr::enableValidationLayers)
        DebugMessengerMgr::destroyDebugUtilsMessengerExt(instance, nullptr);

    SurfaceMgr::destroySurface(instance);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void HelloTriangleApplication::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "17_Vulkan_Window", nullptr, nullptr);
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

void HelloTriangleApplication::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = GraphicsPipelineMgr::renderPass;
    renderPassInfo.framebuffer = FrameBuffersMgr::swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = SwapChainMgr::imageExtent;

    VkClearValue clearValue{{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearValue;
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineMgr::graphicsPipeline);
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(SwapChainMgr::imageExtent.width);
    viewport.height = static_cast<float>(SwapChainMgr::imageExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = SwapChainMgr::imageExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer!");
    }
}