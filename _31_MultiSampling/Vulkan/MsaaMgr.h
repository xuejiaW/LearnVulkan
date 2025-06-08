#pragma once
#include "Utils/ImageHelper.h"

class MsaaMgr
{
public:
    static VkImage colorImage;
    static VkImageView colorImageView;
    static VkDeviceMemory colorImageMemory;

    static void createColorResources();
    static void destroyColorResources();
};