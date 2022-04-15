#pragma once

#include "ember/graphics/image.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_image : public image {
        //VARIABLES
        //FUNCTIONS
    public:
        vulkan_image(VkDevice device, VkImage image, descriptor descriptor) {
            //TODO
        }
       
        static format convert_format(VkFormat format);
    };
}