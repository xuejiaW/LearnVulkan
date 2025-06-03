#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>

class DescriptorMgr
{
public:
    static void createDescriptorSetLayout();
    static void destroyDescriptorSetLayout();

    static void createDescriptorPool();
    static void destroyDescriptorPool();

    static void createDescriptorSets();

    static VkDescriptorSetLayout descriptorSetLayout;
    static std::vector<VkDescriptorSet> descriptorSets;
    static VkDescriptorPool descriptorPool;

};

