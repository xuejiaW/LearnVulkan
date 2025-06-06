#pragma once
#include <vulkan/vulkan_core.h>
#include <string>

class GraphicsPipelineMgr
{
public:
    static void createGraphicsPipeline(const std::string& vertFileName, const std::string& fragFileName);
    static void destroyPipelineLayout();
    static VkPipelineLayout pipelineLayout;

private:
    static VkPipelineShaderStageCreateInfo getShaderStageCreateInfo(VkShaderModule shaderModule, VkShaderStageFlagBits stage);
    static VkPipelineVertexInputStateCreateInfo getVertexInputStateCreateInfo();
    static VkPipelineInputAssemblyStateCreateInfo getInputAssemblyStateCreateInfo();
    static VkPipelineViewportStateCreateInfo getViewportStateCreateInfo();
    static VkPipelineRasterizationStateCreateInfo getRasterizationStateCreateInfo();
    static VkPipelineMultisampleStateCreateInfo getMultisamplingStateCreateInfo();
    static VkPipelineColorBlendStateCreateInfo getColorBlendStateCreateInfo();
    static VkPipelineDynamicStateCreateInfo getVKDynamicStateCreateInfo();
    static VkPipelineLayoutCreateInfo getPipelineLayoutCreateInfo();
};