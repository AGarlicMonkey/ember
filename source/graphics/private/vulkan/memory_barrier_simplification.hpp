#pragma once

#include "ember/graphics/buffer.hpp"
#include "ember/graphics/image.hpp"

#include <vulkan/vulkan.h>

namespace ember::inline graphics {
    struct buffer_access_info {
        VkPipelineStageFlags stage_mask;
        VkAccessFlags access_mask;
    };

    struct image_access_info {
        VkPipelineStageFlags stage_mask;
        VkAccessFlags access_mask;
        VkImageLayout image_layout;
    };
}

namespace ember::inline graphics {
    buffer_access_info get_buffer_access(buffer::access const access);
    image_access_info get_image_access(image::access const access);
}