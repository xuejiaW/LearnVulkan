#pragma once
#include <string>

class GraphicsPipelineMgr
{
public:
    static void createGraphicsPipeline(const std::string& vertFileName, const std::string& fragFileName);
};