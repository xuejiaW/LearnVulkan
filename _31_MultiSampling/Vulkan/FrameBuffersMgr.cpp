#include "FrameBuffersMgr.h"

#include <array>
#include <stdexcept>

#include "DepthBufferMgr.h"
#include "LogicalDevicesMgr.h"
#include "MsaaMgr.h"
#include "GraphicPipeline/GraphicsPipelineMgr.h"
#include "SwapChain/SwapChainMgr.h"

std::vector<VkFramebuffer> FrameBuffersMgr::swapChainFramebuffers{};


void FrameBuffersMgr::createFramebuffers()
{
    swapChainFramebuffers.resize(SwapChainMgr::imageViews.size());

    for (size_t i = 0; i < SwapChainMgr::imageViews.size(); i++)
    {
        std::array<VkImageView, 3> attachments = {MsaaMgr::colorImageView, DepthBufferMgr::depthImageView,SwapChainMgr::imageViews[i]};
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = GraphicsPipelineMgr::renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
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


