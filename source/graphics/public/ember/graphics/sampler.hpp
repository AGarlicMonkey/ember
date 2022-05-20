#pragma once

namespace ember::graphics {
    /**
     * @brief Describes how a shader should sample an image
     */
    class sampler {
        //TYPES
    public:
        enum class filter {
            nearest, /**< Choose the nearest texel to the sample location. */
            linear,  /**< Linearly interpolate between the texels near the sample location. */
        };

        enum class address_mode {
            repeat,
            mirrored_repeat,
            clamp_to_edge,   /**< Uses the colour of the closest edge to the edge of target. */
            clamp_to_border, /**< Uses a solid colour to stretch to the edge of the target. */
        };

        struct descriptor {
            filter min_filter{ filter::nearest };                /**< Filter method to use when minifying the image (less pixels than texels). */
            filter mag_filter{ filter::nearest };                /**< Filter method to use when magnifying the image (more pixels than texels). */
            filter mip_filter{ filter::nearest };                /**< Filter method to use when sampling between mips. */
            address_mode address_mode_u{ address_mode::repeat }; /**< What address mode to use on the image's U coordinate when sampling outside of an image's bounds. */
            address_mode address_mode_v{ address_mode::repeat }; /**< What address mode to use on the image's V coordinate when sampling outside of an image's bounds. */
            address_mode address_mode_w{ address_mode::repeat }; /**< What address mode to use on the image's W coordinate when sampling outside of an image's bounds. */
            bool enable_anisotropy{ true };                      /**< Improves aliasing when undersampling an image. Usually enabled unless specifically required not to. */
        };

        //FUNCTIONS
    public:
        virtual ~sampler() = default;

        virtual const descriptor &get_descriptor() const = 0;
    };
}