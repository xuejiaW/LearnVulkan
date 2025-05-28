#include "UniformBufferMgr.h"
#include <vulkan/vulkan_core.h>

#include <chrono>
#include <stdexcept>

#include "UniformBufferObject.h"
#include "../LogicalDevicesMgr.h"
#include "../GraphicPipeline/GraphicsPipelineMgr.h"
#include "../Utils/BufferHelper.h"
#include "../SwapChain/SwapChainMgr.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

VkDescriptorSetLayout UniformBufferMgr::descriptorSetLayout{};
std::vector<VkBuffer> UniformBufferMgr::uniformBuffers{};
std::vector<VkDeviceMemory> UniformBufferMgr::uniformBuffersMemory{};
std::vector<void*> UniformBufferMgr::uniformBuffersMapped{};
VkDescriptorPool UniformBufferMgr::descriptorPool{};
std::vector<VkDescriptorSet> UniformBufferMgr::descriptorSets{};

void UniformBufferMgr::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1; // Number of resources (e.g., uniform buffers) bound to this binding
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Shader stage(s) that will use this binding

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(LogicalDevicesMgr::device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor set layout!");
    }
}

void UniformBufferMgr::createUniformBuffers()
{
    uniformBuffers.resize(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i != GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT; ++i)
    {
        constexpr VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        BufferHelper::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                   uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(LogicalDevicesMgr::device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}

void UniformBufferMgr::destroyUniformBuffers()
{
    for (size_t i = 0; i != GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT; ++i)
    {
        vkDestroyBuffer(LogicalDevicesMgr::device, uniformBuffers[i], nullptr);
        vkFreeMemory(LogicalDevicesMgr::device, uniformBuffersMemory[i], nullptr);
    }
}

void UniformBufferMgr::updateUniformBuffer(uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const float time = std::chrono::duration<float>(currentTime - startTime).count();

    UniformBufferObject ubo;
    const VkExtent2D& extent = SwapChainMgr::imageExtent;
    ubo.model = rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(extent.width) / static_cast<float>(extent.height), 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;
    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void UniformBufferMgr::createDescriptorPool()
{
    VkDescriptorPoolSize poolSize;
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT); // Every frame will have its own descriptor set

    if (vkCreateDescriptorPool(LogicalDevicesMgr::device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool!");
    }
}

void UniformBufferMgr::destroyDescriptorPool()
{
    vkDestroyDescriptorPool(LogicalDevicesMgr::device, descriptorPool, nullptr);
}

void UniformBufferMgr::createDescriptorSets()
{
    descriptorSets.resize(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT);

    const std::vector layouts(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    if (vkAllocateDescriptorSets(LogicalDevicesMgr::device, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT; ++i)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(LogicalDevicesMgr::device, 1, &descriptorWrite, 0, nullptr);
    }
}


void UniformBufferMgr::destroyDescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(LogicalDevicesMgr::device, descriptorSetLayout, nullptr);
}
