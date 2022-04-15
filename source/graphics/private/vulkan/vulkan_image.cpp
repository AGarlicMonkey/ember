#include "vulkan_image.hpp"

#include "log.hpp"

namespace ember::graphics {
    image::format vulkan_image::convert_format(VkFormat format) {
        switch(format) {
            case VK_FORMAT_R8_UNORM:
                return format::R8_UNORM;
            case VK_FORMAT_R8G8B8A8_SRGB:
                return format::R8G8B8A8_SRGB;
            case VK_FORMAT_B8G8R8A8_SRGB:
                return format::B8G8R8A8_SRGB;
            case VK_FORMAT_B8G8R8A8_UNORM:
                return format::B8G8R8A8_UNORM;
            case VK_FORMAT_D32_SFLOAT:
                return format::D32_SFLOAT;
            default:
                EMBER_CHECK_MSG(false, "{0}: Format not supported.", EMBER_FUNCTION_NAME);
                return format::unknown;
        }
    }
}