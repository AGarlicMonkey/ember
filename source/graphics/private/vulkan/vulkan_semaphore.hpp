#pragma once

#include "ember/graphics/semaphore.hpp"
#include "vulkan/vulkan.h"

namespace ember::graphics {
    class vulkan_semaphore : public semaphore {
        //VARIABLES
        //FUNCTIONS
    public:
        inline VkSemaphore get_semaphore() const;
    };
}

#include "vulkan_semaphore.inl"