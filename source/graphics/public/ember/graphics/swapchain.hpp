#pragma once

#include "ember/graphics/image.hpp"

#include <ember/containers/array.hpp>
#include <ember/maths/vector.hpp>
#include <utility>

namespace ember::graphics {
    class semaphore;
}

namespace ember::graphics {
    /**
     * @brief Facilitates the presentation of rendering results to a window.
     */
    class swapchain {
        //TYPES
    public:
        enum class result {
            success_swapchain_suboptimal,
            success = 0,

            error,
            error_swapchain_out_of_date,
        };

        struct descriptor {
            maths::vec2u extent{};                                                 /**< Size of the swapchain's backing images. */
            std::uint32_t image_count{ 3 };                                        /**< How many images this swapchain will hold. */
            image::usage_mode usage_flags{ image::usage_mode::colour_attachment }; /** Usage flags for the swapchain's internal images. */
        };

        //FUNCTIONS
    public:
        virtual ~swapchain() = default;

        /**
         * @brief Aquire the next available image that can be rendered to.
         * @param available_semaphore A Sempahore that the swapchain will signal when the image is ready, can be nullptr.
         * @return Returns a pair that contains an index to the next image that is ready to render to and a
         * result for if the operation was successful.
         */
        virtual std::pair<std::uint32_t, result> aquire_next_image(semaphore const *available_semaphore) = 0;

        /**
         * @brief
         * @return 
         */
        virtual image::format get_image_format() const = 0;
        /**
         * @brief
         * @return 
         */
        virtual maths::vec2u get_size() const = 0;

        /**
         * @brief Return the image views backing this swapchain. The lifetime
         * of the views are tied to this object.
         * @return 
         */
        virtual containers::array<image *> get_images() const = 0;
    };
}