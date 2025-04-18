#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

class SurfaceMgr
{
public:
    static void createSurface(VkInstance instance, GLFWwindow* window);
    static void destroySurface(VkInstance instance);
    static VkSurfaceKHR surface;
};








