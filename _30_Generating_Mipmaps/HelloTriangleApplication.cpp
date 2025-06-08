// ReSharper disable CppUnreachableCode
// ReSharper disable CppRedundantBooleanExpressionArgument
#include "HelloTriangleApplication.h"

#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <stdexcept>
#include <vector>

#include "Vulkan/DebugMessengerMgr.h"
#include "Vulkan/DepthBufferMgr.h"
#include "Vulkan/DescriptorMgr.h"
#include "Vulkan/ExtensionsMgr.h"
#include "Vulkan/FrameBuffersMgr.h"
#include "Vulkan/LogicalDevicesMgr.h"
#include "Vulkan/PhysicalDevicesMgr.h"
#include "Vulkan/SurfaceMgr.h"
#include "Vulkan/SyncObjectsMgr.h"
#include "Vulkan/ValidationLayerMgr.h"
#include "Vulkan/CommandBuffers/CommandBuffersMgr.h"
#include "Vulkan/GraphicPipeline/GraphicsPipelineMgr.h"
#include "Vulkan/Models/ModelsMgr.h"
#include "Vulkan/SwapChain/SwapChainMgr.h"
#include "Vulkan/Textures/TextureMgr.h"
#include "Vulkan/UniformBuffer/UniformBufferMgr.h"
#include "Vulkan/Vertex/VertexDataMgr.h"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

GLFWwindow* HelloTriangleApplication::window = nullptr;
uint32_t HelloTriangleApplication::currentFrame = 0;
bool framebufferResized = false;

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
    DescriptorMgr::createDescriptorSetLayout();
    DepthBufferMgr::createDepthResources();
    GraphicsPipelineMgr::createGraphicsPipeline("Shaders/TriangleVert.spv", "Shaders/TriangleFrag.spv");
    FrameBuffersMgr::createFramebuffers();
    CommandBuffersMgr::createCommandPool();
    TextureMgr::createTextureImage("../Textures/viking_room.png");
    TextureMgr::createTextureImageView();
    TextureMgr::createTextureSampler();
    ModelsMgr::loadModel();
    VertexDataMgr::createVertexBuffer();
    VertexDataMgr::createIndexBuffer();
    UniformBufferMgr::createUniformBuffers();
    DescriptorMgr::createDescriptorPool();
    DescriptorMgr::createDescriptorSets();
    CommandBuffersMgr::createCommandBuffers();
    SyncObjectsMgr::createSyncObjects();
}

void HelloTriangleApplication::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(LogicalDevicesMgr::device);
}

void HelloTriangleApplication::cleanup()
{
    SyncObjectsMgr::destroySyncObjects();
    CommandBuffersMgr::destroyCommandPool();
    DescriptorMgr::destroyDescriptorPool();
    UniformBufferMgr::destroyUniformBuffers();
    VertexDataMgr::destroyIndexBuffer();
    VertexDataMgr::destroyVertexBuffer();
    FrameBuffersMgr::destroyFramebuffers();
    GraphicsPipelineMgr::destroyGraphicsPipeline();
    DepthBufferMgr::destroyDepthResources();
    DescriptorMgr::destroyDescriptorSetLayout();
    SwapChainMgr::destroyImageViews();
    SwapChainMgr::destroySwapChain();
    TextureMgr::destroyTextureSampler();
    TextureMgr::destroyTextureImageView();
    TextureMgr::destroyTextureImage();
    LogicalDevicesMgr::destroyLogicalDevice();
    if (ValidationLayerMgr::enableValidationLayers)
        DebugMessengerMgr::destroyDebugUtilsMessengerExt(instance, nullptr);

    SurfaceMgr::destroySurface(instance);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    framebufferResized = true;
}

void HelloTriangleApplication::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "30_Vulkan_Window", nullptr, nullptr);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
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

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}}; // Clear color to white
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = clearValues.size();
    renderPassInfo.pClearValues = clearValues.data();
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineMgr::graphicsPipeline);
    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(SwapChainMgr::imageExtent.width);
    viewport.height = static_cast<float>(SwapChainMgr::imageExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor;
    scissor.offset = {0, 0};
    scissor.extent = SwapChainMgr::imageExtent;

    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    const VkBuffer vertexBuffers[] = {VertexDataMgr::vertexBuffer};
    constexpr VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, VertexDataMgr::indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            GraphicsPipelineMgr::pipelineLayout, 0, 1,
                            &DescriptorMgr::descriptorSets[currentFrame], 0, nullptr);

    vkCmdDrawIndexed(commandBuffer,
                     static_cast<uint32_t>(VertexDataMgr::indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer!");
    }
}

void HelloTriangleApplication::drawFrame()
{
    vkWaitForFences(LogicalDevicesMgr::device, 1, &SyncObjectsMgr::inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex = 0;
    VkResult result = vkAcquireNextImageKHR(LogicalDevicesMgr::device, SwapChainMgr::swapChain, UINT64_MAX,
                                            SyncObjectsMgr::imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE,
                                            &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        SwapChainMgr::recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    UniformBufferMgr::updateUniformBuffer(currentFrame);

    vkResetFences(LogicalDevicesMgr::device, 1, &SyncObjectsMgr::inFlightFences[currentFrame]);

    vkResetCommandBuffer(CommandBuffersMgr::commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    recordCommandBuffer(CommandBuffersMgr::commandBuffers[currentFrame], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    const VkSemaphore waitSemaphores[] = {SyncObjectsMgr::imageAvailableSemaphores[currentFrame]};
    constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &CommandBuffersMgr::commandBuffers[currentFrame];

    const VkSemaphore signalSemaphores[] = {SyncObjectsMgr::renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(LogicalDevicesMgr::graphicsQueue, 1, &submitInfo, SyncObjectsMgr::inFlightFences[currentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    const VkSwapchainKHR swapChains[] = {SwapChainMgr::swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(LogicalDevicesMgr::graphicsQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
    {
        framebufferResized = false;
        SwapChainMgr::recreateSwapChain();
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to present!");
    }

    currentFrame = (currentFrame + 1) % GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT;
}
