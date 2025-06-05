#include "DescriptorMgr.h"

#include <array>
#include <stdexcept>

#include "LogicalDevicesMgr.h"
#include "GraphicPipeline/GraphicsPipelineMgr.h"
#include "Textures/TextureMgr.h"
#include "UniformBuffer/UniformBufferMgr.h"
#include "UniformBuffer/UniformBufferObject.h"

VkDescriptorSetLayout DescriptorMgr::descriptorSetLayout{};
VkDescriptorPool DescriptorMgr::descriptorPool{};
std::vector<VkDescriptorSet> DescriptorMgr::descriptorSets{};

void DescriptorMgr::createDescriptorPool()
{
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(GraphicsPipelineMgr::MAX_FRAMES_IN_FLIGHT); // Every frame will have its own descriptor set

    if (vkCreateDescriptorPool(LogicalDevicesMgr::device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool!");
    }
}

void DescriptorMgr::destroyDescriptorPool()
{
    vkDestroyDescriptorPool(LogicalDevicesMgr::device, descriptorPool, nullptr);
}

void DescriptorMgr::createDescriptorSets()
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
        bufferInfo.buffer = UniformBufferMgr::uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = TextureMgr::textureImageView;
        imageInfo.sampler = TextureMgr::textureSampler;

        std::array<VkWriteDescriptorSet, 2> writeDescriptorSets{};
        writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSets[0].dstSet = descriptorSets[i];
        writeDescriptorSets[0].dstBinding = 0;
        writeDescriptorSets[0].dstArrayElement = 0;
        writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSets[0].descriptorCount = 1;
        writeDescriptorSets[0].pBufferInfo = &bufferInfo;

        writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSets[1].dstSet = descriptorSets[i];
        writeDescriptorSets[1].dstBinding = 1;
        writeDescriptorSets[1].dstArrayElement = 0;
        writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeDescriptorSets[1].descriptorCount = 1;
        writeDescriptorSets[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(LogicalDevicesMgr::device, writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
    }
}

void DescriptorMgr::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1; // Number of resources (e.g., uniform buffers) bound to this binding
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Shader stage(s) that will use this binding

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(LogicalDevicesMgr::device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor set layout!");
    }
}

void DescriptorMgr::destroyDescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(LogicalDevicesMgr::device, descriptorSetLayout, nullptr);
}



