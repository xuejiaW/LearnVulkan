#include "QueueFamilyIndices.h"

bool QueueFamilyIndices::isComplete()
{
    return graphicsFamily.has_value();
}

