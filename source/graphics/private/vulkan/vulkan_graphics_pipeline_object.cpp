#include "vulkan_graphics_pipeline_object.hpp"

#include "ember/core/log.hpp"

namespace ember::inline graphics {
    graphics_pipeline_object::descriptor const &vulkan_graphics_pipeline_object::get_descriptor() const {
        return desc;
    }

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

    VkCompareOp vulkan_graphics_pipeline_object::convert_compare_op(compare_op const op) {
        switch(op) {
            case compare_op::less:
                return VK_COMPARE_OP_LESS;
            case compare_op::less_or_equal:
                return VK_COMPARE_OP_LESS_OR_EQUAL;
            default:
                EMBER_CHECK(false);
                return VK_COMPARE_OP_NEVER;
        }
    }
}