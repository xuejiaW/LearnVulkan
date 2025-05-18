#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>

class UniformBufferMgr
{
public:
    static void createDescriptorSetLayout();
    static void destroyDescriptorSetLayout();

    static void createUniformBuffers();
    static void destroyUniformBuffers();

    static void createDescriptorPool();
    static void destroyDescriptorPool();
    
    static void createDescriptorSets();

    static void updateUniformBuffer(uint32_t currentImage);

    static VkDescriptorSetLayout descriptorSetLayout;
    static std::vector<VkDescriptorSet> descriptorSets;

    static VkDescriptorPool descriptorPool;
    static std::vector<VkBuffer> uniformBuffers;
    static std::vector<VkDeviceMemory> uniformBuffersMemory;
    static std::vector<void*> uniformBuffersMapped;
};


