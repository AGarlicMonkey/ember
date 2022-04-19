#pragma once

#include <cinttypes>

namespace ember::graphics {
    /**
     * @brief Views a subset of an image.
     * @details images are not directly accessed by shaders for reading / writing.
     * Instead, a view onto that image is used.
     */
    class image_view {
        //TYPES
    public:
        enum class type {
            _2d,
            _2d_array,
            _3d,
            cube,
            cube_array,
        };

        struct descriptor {
            type type{ type::_2d };
            std::uint32_t layer{ 0 };       /**< If the image is an array or cube map, which element to start on (0 indexed). */
            std::uint32_t layer_count{ 1 }; /**< If the image is an array or cube map, how many elements to view. */
        };

        //FUNCTIONS
    public:
        virtual ~image_view() = default;
    };
}