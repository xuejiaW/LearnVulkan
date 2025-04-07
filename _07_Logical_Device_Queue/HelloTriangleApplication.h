#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <vector>

class HelloTriangleApplication
{
public:
    void run();

private:
    void initVulkan();
    void initWindow();
    void mainLoop();
    void cleanup();

    void createInstance();

    GLFWwindow* window = nullptr;
    VkInstance instance = nullptr;
};




