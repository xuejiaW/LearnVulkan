#include "GraphicsPipelineMgr.h"

#include "Shaders/ShadersMgr.h"


void GraphicsPipelineMgr::createGraphicsPipeline(const std::string& vertFileName, const std::string& fragFileName)
{
    VkShaderModule vertShaderModule = ShadersMgr::createShaderModule(vertFileName);
    VkShaderModule fragShaderModule = ShadersMgr::createShaderModule(fragFileName);

    VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo = {};
    vertShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageCreateInfo.module = vertShaderModule;
    vertShaderStageCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageCreateInfo = {};
    fragShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageCreateInfo.module = fragShaderModule;
    fragShaderStageCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageCreateInfo, fragShaderStageCreateInfo};

    ShadersMgr::destroyShaderModule(vertShaderModule);
    ShadersMgr::destroyShaderModule(fragShaderModule);
}