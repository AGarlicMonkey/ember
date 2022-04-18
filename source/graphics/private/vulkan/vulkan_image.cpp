#include "vulkan_image.hpp"

#include "log.hpp"

namespace ember::graphics {
    image::format vulkan_image::convert_format(VkFormat format) {
        switch(format) {
            case VK_FORMAT_R8_UNORM:
                return format::R8_UNORM;
            case VK_FORMAT_R32G32_UINT:
                return format::R32G32_UINT;
            case VK_FORMAT_R8G8B8A8_SRGB:
                return format::R8G8B8A8_SRGB;
            case VK_FORMAT_B8G8R8A8_SRGB:
                return format::B8G8R8A8_SRGB;
            case VK_FORMAT_B8G8R8A8_UNORM:
                return format::B8G8R8A8_UNORM;
            case VK_FORMAT_D32_SFLOAT:
                return format::D32_SFLOAT;
            default:
                EMBER_CHECK(false);
                return format::unknown;
        }
    }

    VkFormat vulkan_image::convert_format(format format) {
        switch(format) {
            case format::R8_UNORM:
                return VK_FORMAT_R8_UNORM;
            case format::R32G32_UINT:
                return VK_FORMAT_R32G32_UINT;
            case format::R8G8B8A8_SRGB:
                return VK_FORMAT_R8G8B8A8_SRGB;
            case format::B8G8R8A8_SRGB:
                return VK_FORMAT_B8G8R8A8_SRGB;
            case format::B8G8R8A8_UNORM:
                return VK_FORMAT_B8G8R8A8_UNORM;
            case format::D32_SFLOAT:
                return VK_FORMAT_D32_SFLOAT;
            default:
                EMBER_CHECK(false);
                return VK_FORMAT_UNDEFINED;
        }
    }

    VkImageLayout vulkan_image::convert_layout(layout layout) {
        switch(layout) {
            case layout::undefined:
                return VK_IMAGE_LAYOUT_UNDEFINED;
            case layout::general:
                return VK_IMAGE_LAYOUT_GENERAL;
            case layout::present:
                return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            case layout::transfer_source_optimal:
                return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            case layout::transfer_destination_optimal:
                return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            case layout::shader_read_only_optimal:
                return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case layout::colour_attachment_optimal:
                return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case layout::depth_stencil_attachment_optimal:
                return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case layout::cepth_stencil_read_only_optimal:
                return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            default:
                EMBER_CHECK(false);
                return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }
}