#include "vulkan_image_view.hpp"

#include <ember/core/log.hpp>

namespace ember::inline graphics {
    image_view::descriptor const &vulkan_image_view::get_descriptor() const {
        return desc;
    }

    VkImageViewType vulkan_image_view::convert_type(type const type) {
        switch(type) {
            case image_view::type::_2d:
                return VK_IMAGE_VIEW_TYPE_2D;
            case image_view::type::_2d_array:
                return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
            case image_view::type::_3d:
                return VK_IMAGE_VIEW_TYPE_3D;
            case image_view::type::cube:
                return VK_IMAGE_VIEW_TYPE_CUBE;
            case image_view::type::cube_array:
                return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
            default:
                EMBER_CHECK(false);
                return VK_IMAGE_VIEW_TYPE_2D;
        }
    }
}