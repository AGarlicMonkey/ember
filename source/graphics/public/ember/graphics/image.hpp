#pragma once

#include "ember/graphics/resource.hpp"

#include <cinttypes>
#include <ember/maths/vector.hpp>

namespace ember::graphics {
    class image {
        //TYPES
    public:
        enum class usage_mode : std::uint8_t {
            transfer_source          = 1 << 0, /**< Image will be used as a source in a transfer operation. */
            transfer_destination     = 1 << 1, /**< Image will be used as a destination in a transfer operation. */
            sampled                  = 1 << 2, /**< Image will be sampled in a shader. */
            storage                  = 1 << 3, /**< Image requires unorderred access (such as writing to from a compute shader). */
            colour_attachment        = 1 << 4, /**< Image will be a colour attachment in a framebuffer.  */
            depth_stencil_attachment = 1 << 5, /**< Image will be for a depth / stencil attachment in a framebuffer */
        };

        enum class type {
            _2d,
            _3d,
            cube
        };

        enum class format {
            unknown,

            R8_UNORM,
            R32G32_UINT,
            R8G8B8A8_SRGB,
            B8G8R8A8_SRGB,
            B8G8R8A8_UNORM,

            D32_SFLOAT
        };

        enum class layout {
            undefined,
            general,
            present,
            transfer_sSource_optimal,
            transfer_destination_optimal,
            shader_read_only_optimal,
            colour_attachment_optimal,
            depth_stencil_attachment_optimal,
            cepth_stencil_readOnly_optimal
        };

        struct descriptor {
            //TODO: resource name -- or should this be on the factory call as it should ber unique per object
            type type{ type::_2d };
            usage_mode usage_flags;
            maths::vec2u dimensions;
            std::uint32_t array_count{ 1u }; /**< How many elements to create for an image array. */
            format format;
            sharing_mode sharing_mode{ sharing_mode::exclusive };
        };

        //TODO
    };
}