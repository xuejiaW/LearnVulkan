#pragma once
#include "QueueFamilyIndices.h"
#include "../PhysicalDevicesMgr.h"

class QueueFamilyMgr
{
public:
    static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
};

