#pragma once

#include <vulkan/vulkan_core.h>

#include <string>
#include <vector>

class ShadersMgr
{
public:
    static VkShaderModule createShaderModule(const std::string& fileName);
    static void destroyShaderModule(VkShaderModule shaderModule);

private:
    static std::vector<char> readFile(const std::string& fileName);
};

