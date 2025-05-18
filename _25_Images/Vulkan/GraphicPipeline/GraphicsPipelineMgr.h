#pragma once
#include <vulkan/vulkan_core.h>
#include <string>

class GraphicsPipelineMgr
{
public:
    static void createGraphicsPipeline(const std::string& vertFileName, const std::string& fragFileName);

    static void destroyGraphicsPipeline();

    static VkPipeline graphicsPipeline;
    static VkRenderPass renderPass;
    static VkPipelineLayout pipelineLayout;
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

private:
    static void destroyRenderPass();
    static void destroyPipelineLayout();
    static void createRenderPass();
    static void createPipelineLayout();
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






