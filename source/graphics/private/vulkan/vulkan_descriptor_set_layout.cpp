#include "vulkan_descriptor_set_layout.hpp"

#include <ember/core/log.hpp>

namespace ember::graphics {
    descriptor_set_layout::descriptor const &vulkan_descriptor_set_layout::get_descriptor() const {
        return desc;
    }

    VkDescriptorType vulkan_descriptor_set_layout::convert_descriptor_type(descriptor_type const type) {
        switch(type) {
            case descriptor_type::uniform_buffer:
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            case descriptor_type::storage_buffer:
                return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            case descriptor_type::sampled_image:
                return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            case descriptor_type::storage_image:
                return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            case descriptor_type::sampler:
                return VK_DESCRIPTOR_TYPE_SAMPLER;
            default:
                EMBER_CHECK(false);
                return VK_DESCRIPTOR_TYPE_MAX_ENUM;
        }
    }
}