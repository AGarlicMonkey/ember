#include "vulkan_sampler.hpp"

#include "log.hpp"

namespace ember::inline graphics {
    sampler::descriptor const &vulkan_sampler::get_descriptor() const {
        return desc;
    }

    VkFilter vulkan_sampler::convert_filter(filter filter) {
        switch(filter) {
            case filter::nearest:
                return VK_FILTER_NEAREST;
            case filter::linear:
                return VK_FILTER_LINEAR;
            default:
                EMBER_CHECK(false);
                return VK_FILTER_NEAREST;
        }
    }

    VkSamplerMipmapMode vulkan_sampler::convert_mipmap_mode(filter filter) {
        switch(filter) {
            case filter::nearest:
                return VK_SAMPLER_MIPMAP_MODE_NEAREST;
            case filter::linear:
                return VK_SAMPLER_MIPMAP_MODE_LINEAR;
            default:
                EMBER_CHECK(false);
                return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        }
    }

    VkSamplerAddressMode vulkan_sampler::convert_address_mode(address_mode mode) {
        switch(mode){
            case address_mode::repeat:
                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            case address_mode::mirrored_repeat:
                return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            case address_mode::clamp_to_edge:
                return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            case address_mode::clamp_to_border:
                return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            default:
                EMBER_CHECK(false);
                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        }
    }
}