#pragma once

#include <cinttypes>
#include <ember/containers/array.hpp>

namespace ember::graphics {
    class render_pass;
    class image_view;
}

namespace ember::graphics {
    /**
     * @brief Provides attachments to a given render_pass
     */
    class framebuffer {
        //TYPES
    public:
        struct descriptor {
            render_pass const *render_pass{ nullptr };
            containers::array<image_view const *> attachments{}; /**< Order of attachments is expected to match */
            std::uint32_t width{ 0 };
            std::uint32_t height{ 0 };
        };

        //FUNCTIONS
    public:
        virtual ~framebuffer() = default;

        virtual descriptor const &get_descriptor() const = 0;
    };
}