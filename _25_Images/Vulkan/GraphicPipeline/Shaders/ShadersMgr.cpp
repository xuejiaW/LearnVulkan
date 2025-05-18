#include "ShadersMgr.h"
#include <fstream>
#include <iostream>

#include "../../LogicalDevicesMgr.h"

std::vector<char> ShadersMgr::readFile(const std::string& fileName)
{
    std::ifstream file(fileName, std::ios::ate | std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file " + fileName);

    const size_t fileSize = file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}


VkShaderModule ShadersMgr::createShaderModule(const std::string& fileName)
{
    auto code = readFile(fileName);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(LogicalDevicesMgr::device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        throw std::runtime_error("Failed to create shader module!");

    return shaderModule;
}

void ShadersMgr::destroyShaderModule(VkShaderModule shaderModule)
{
    vkDestroyShaderModule(LogicalDevicesMgr::device, shaderModule, nullptr);
}












