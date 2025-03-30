#pragma once
#include <GLFW/glfw3.h>

class HelloTriangleApplication
{
public:
    void run();

private:
    void initVulkan();
    void initWindow();
    void mainLoop();
    void cleanup();
    GLFWwindow* window;
};
