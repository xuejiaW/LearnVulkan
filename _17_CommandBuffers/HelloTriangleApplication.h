#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

class HelloTriangleApplication
{
public:
    void run();
    static GLFWwindow* window;

private:
    void initVulkan();
    void initWindow();
    void mainLoop();
    void cleanup();

    void createInstance();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    VkInstance instance = nullptr;
};