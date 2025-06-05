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
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Textures/TextureMgr.h"

std::vector<VkBuffer> UniformBufferMgr::uniformBuffers{};
std::vector<VkDeviceMemory> UniformBufferMgr::uniformBuffersMemory{};
std::vector<void*> UniformBufferMgr::uniformBuffersMapped{};

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
    ubo.view = lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(extent.width) / static_cast<float>(extent.height), 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;
    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}



