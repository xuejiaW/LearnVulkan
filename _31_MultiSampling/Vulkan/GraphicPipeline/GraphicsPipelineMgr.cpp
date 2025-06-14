#include "GraphicsPipelineMgr.h"

#include <stdexcept>

#include "../DepthBufferMgr.h"
#include "../DescriptorMgr.h"
#include "../LogicalDevicesMgr.h"
#include "../MsaaMgr.h"
#include "../PhysicalDevicesMgr.h"
#include "Shaders/ShadersMgr.h"
#include "../SwapChain/SwapChainMgr.h"
#include "../Vertex/Vertex.h"

VkPipeline GraphicsPipelineMgr::graphicsPipeline = nullptr;
VkPipelineLayout GraphicsPipelineMgr::pipelineLayout = nullptr;
VkRenderPass GraphicsPipelineMgr::renderPass = nullptr;


void GraphicsPipelineMgr::createGraphicsPipeline(const std::string& vertFileName, const std::string& fragFileName)
{
    createRenderPass();
    createPipelineLayout();

    // Create shader modules
    VkShaderModule vertShaderModule = ShadersMgr::createShaderModule(vertFileName);
    VkShaderModule fragShaderModule = ShadersMgr::createShaderModule(fragFileName);

    // Create shader stages
    VkPipelineShaderStageCreateInfo shaderStages[] = {getShaderStageCreateInfo(vertShaderModule, VK_SHADER_STAGE_VERTEX_BIT),
                                                      getShaderStageCreateInfo(fragShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT)};

    // Fixed functions
    VkPipelineVertexInputStateCreateInfo vertexInput = getVertexInputStateCreateInfo();
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = getInputAssemblyStateCreateInfo();
    VkPipelineViewportStateCreateInfo viewportState = getViewportStateCreateInfo();
    VkPipelineRasterizationStateCreateInfo rasterizer = getRasterizationStateCreateInfo();
    VkPipelineMultisampleStateCreateInfo multisampling = getMultisamplingStateCreateInfo();
    VkPipelineColorBlendStateCreateInfo colorBlending = getColorBlendStateCreateInfo();
    VkPipelineDynamicStateCreateInfo dynamicState = getVKDynamicStateCreateInfo();
    VkPipelineDepthStencilStateCreateInfo depthStencil = getDepthStencilStateCreateInfo();

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = shaderStages;
    pipelineCreateInfo.pVertexInputState = &vertexInput;
    pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pRasterizationState = &rasterizer;
    pipelineCreateInfo.pMultisampleState = &multisampling;
    pipelineCreateInfo.pDepthStencilState = nullptr;
    pipelineCreateInfo.pColorBlendState = &colorBlending;
    pipelineCreateInfo.pDynamicState = &dynamicState;
    pipelineCreateInfo.pDepthStencilState = &depthStencil;
    pipelineCreateInfo.layout = pipelineLayout;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.renderPass = renderPass;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(LogicalDevicesMgr::device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
        throw std::runtime_error("failed to create graphics pipeline");

    // // Clean up shader modules
    ShadersMgr::destroyShaderModule(vertShaderModule);
    ShadersMgr::destroyShaderModule(fragShaderModule);
}

void GraphicsPipelineMgr::createPipelineLayout()
{
    const VkPipelineLayoutCreateInfo pipelineLayoutInfo = getPipelineLayoutCreateInfo();
    if (vkCreatePipelineLayout(LogicalDevicesMgr::device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        throw std::runtime_error("Failed to create pipeline layout!");
}


void GraphicsPipelineMgr::destroyGraphicsPipeline()
{
    vkDestroyPipeline(LogicalDevicesMgr::device, graphicsPipeline, nullptr);
    destroyPipelineLayout();
    destroyRenderPass();
}

VkPipelineDepthStencilStateCreateInfo GraphicsPipelineMgr::getDepthStencilStateCreateInfo()
{
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;
    return depthStencil;
}


VkPipelineShaderStageCreateInfo GraphicsPipelineMgr::getShaderStageCreateInfo(VkShaderModule shaderModule, VkShaderStageFlagBits stage)
{
    VkPipelineShaderStageCreateInfo shaderStageInfo = {};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";
    return shaderStageInfo;
}

VkPipelineVertexInputStateCreateInfo GraphicsPipelineMgr::getVertexInputStateCreateInfo()
{
    static auto bindingDescription = Vertex::getBindingDescription();
    static auto attributeDescriptions = Vertex::getAttributeDescriptions();
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo GraphicsPipelineMgr::getInputAssemblyStateCreateInfo()
{
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    return inputAssembly;
}

VkPipelineViewportStateCreateInfo GraphicsPipelineMgr::getViewportStateCreateInfo()
{
    static VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(SwapChainMgr::imageExtent.width);
    viewport.height = static_cast<float>(SwapChainMgr::imageExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    static VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = SwapChainMgr::imageExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    return viewportState;
}

VkPipelineRasterizationStateCreateInfo GraphicsPipelineMgr::getRasterizationStateCreateInfo()
{
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;
    return rasterizer;
}

VkPipelineMultisampleStateCreateInfo GraphicsPipelineMgr::getMultisamplingStateCreateInfo()
{
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.minSampleShading = 0.2f;
    multisampling.rasterizationSamples = PhysicalDevicesMgr::msaaSamples;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;
    return multisampling;
}

VkPipelineColorBlendStateCreateInfo GraphicsPipelineMgr::getColorBlendStateCreateInfo()
{
    static VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                                          VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT |
                                          VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    return colorBlending;
}

VkPipelineDynamicStateCreateInfo GraphicsPipelineMgr::getVKDynamicStateCreateInfo()
{
    static VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;
    return dynamicState;
}

VkPipelineLayoutCreateInfo GraphicsPipelineMgr::getPipelineLayoutCreateInfo()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &DescriptorMgr::descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    return pipelineLayoutInfo;
}

void GraphicsPipelineMgr::destroyPipelineLayout()
{
    vkDestroyPipelineLayout(LogicalDevicesMgr::device, pipelineLayout, nullptr);
}


void GraphicsPipelineMgr::createRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = SwapChainMgr::imageFormat;
    colorAttachment.samples = PhysicalDevicesMgr::msaaSamples;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = DepthBufferMgr::findDepthFormat();
    depthAttachment.samples = PhysicalDevicesMgr::msaaSamples;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format = SwapChainMgr::imageFormat;
    colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment = 2;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    subpass.pResolveAttachments = &colorAttachmentResolveRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstSubpass = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 3> attachments = {colorAttachment, depthAttachment, colorAttachmentResolve};

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(LogicalDevicesMgr::device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create render pass!");
    }
}


void GraphicsPipelineMgr::destroyRenderPass()
{
    vkDestroyRenderPass(LogicalDevicesMgr::device, renderPass, nullptr);
}