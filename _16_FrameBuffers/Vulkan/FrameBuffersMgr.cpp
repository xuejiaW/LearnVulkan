#include "FrameBuffersMgr.h"

#include <stdexcept>

#include "LogicalDevicesMgr.h"
#include "GraphicPipeline/GraphicsPipelineMgr.h"
#include "SwapChain/SwapChainMgr.h"

std::vector<VkFramebuffer> FrameBuffersMgr::swapChainFramebuffers{};


void FrameBuffersMgr::createFramebuffers()
{
    swapChainFramebuffers.resize(SwapChainMgr::imageViews.size());

    for (size_t i = 0; i < SwapChainMgr::imageViews.size(); i++)
    {
        VkImageView attachments[] = {SwapChainMgr::imageViews[i]};
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = GraphicsPipelineMgr::renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = SwapChainMgr::imageExtent.width;
        framebufferInfo.height = SwapChainMgr::imageExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(LogicalDevicesMgr::device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer");
        }
    }
}

void FrameBuffersMgr::destroyFramebuffers()
{
    for (auto framebuffer : swapChainFramebuffers)
    {
        vkDestroyFramebuffer(LogicalDevicesMgr::device, framebuffer, nullptr);
    }
}