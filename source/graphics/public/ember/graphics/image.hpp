#pragma once

#include "ember/graphics/resource.hpp"

#include <cinttypes>
#include <ember/core/enum.hpp>
#include <ember/maths/vector.hpp>

namespace ember::inline graphics {
    class image {
        //TYPES
    public:
        using usage_mode_type = std::uint8_t;
        enum class usage_mode : usage_mode_type {
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
            transfer_source_optimal,
            transfer_destination_optimal,
            shader_read_only_optimal,
            colour_attachment_optimal,
            depth_stencil_attachment_optimal,
            depth_stencil_read_only_optimal
        };

        using access_type = std::uint16_t;
        enum class access : access_type {
            none,

            //Read
            pixel_shader_read_sampled,
            depth_stencil_attachment_read,
            compute_shader_read_sampled,
            transfer_read,
            present,

            //Write
            colour_attachment_write,
            depth_stencil_attachment_write,
            compute_shader_write_storage,
            transfer_write,
        };

        struct descriptor {
            type type{ type::_2d };
            usage_mode usage_flags;
            vec2u dimensions;
            std::uint32_t array_count{ 1u }; /**< How many elements to create for an image array. */
            format format;
            sharing_mode sharing_mode{ sharing_mode::exclusive };
        };

        //FUNCTIONS
    public:
        virtual ~image() = default;

        virtual descriptor const &get_descriptor() const = 0;
    };

    EMBER_ENUM_BIT_FLAG_OPERATORS(image::usage_mode, image::usage_mode_type);
}