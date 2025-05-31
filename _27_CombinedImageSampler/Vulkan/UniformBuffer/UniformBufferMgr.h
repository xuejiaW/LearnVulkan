#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>

class UniformBufferMgr
{
public:

    static void createUniformBuffers();
    static void destroyUniformBuffers();

    static void updateUniformBuffer(uint32_t currentImage);

    static std::vector<VkBuffer> uniformBuffers;
    static std::vector<VkDeviceMemory> uniformBuffersMemory;
    static std::vector<void*> uniformBuffersMapped;
};



