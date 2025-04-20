#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>

class FrameBuffersMgr
{
public:
    static void createFramebuffers();
    static void destroyFramebuffers();
    static std::vector<VkFramebuffer> swapChainFramebuffers;
};

