#include "vulkan_graphics_pipeline_object.hpp"

#include "ember/core/log.hpp"

namespace ember::graphics {
    VkFormat vulkan_graphics_pipeline_object::convert_attribute_format(vertex_attribute_format const format) {
        switch(format) {
            case vertex_attribute_format::R32_SFLOAT:
                return VK_FORMAT_R32_SFLOAT;
            case vertex_attribute_format::R32G32_SFLOAT:
                return VK_FORMAT_R32G32_SFLOAT;
            case vertex_attribute_format::R32G32B32_SFLOAT:
                return VK_FORMAT_R32G32B32_SFLOAT;
            case vertex_attribute_format::R32G32B32A32_SFLOAT:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            case vertex_attribute_format::R32G32B32A32_SINT:
                return VK_FORMAT_R32G32B32A32_SINT;
            default:
                EMBER_CHECK(false);
                return VK_FORMAT_UNDEFINED;
        }
    }
}