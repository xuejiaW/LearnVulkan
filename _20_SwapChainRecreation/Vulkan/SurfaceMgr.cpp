#define GLFW_INCLUDE_VULKAN
#include "SurfaceMgr.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

VkSurfaceKHR SurfaceMgr::surface = nullptr;

void SurfaceMgr::createSurface(VkInstance instance, GLFWwindow* window)
{
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}

void SurfaceMgr::destroySurface( VkInstance instance)
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
}












