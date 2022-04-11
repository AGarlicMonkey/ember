#pragma once

#include <ember/core/export.hpp>
#include <ember/maths/vector.hpp>
#include <ember/memory/unique_ptr.hpp>
#include <string>

namespace ember::platform {
    /**
     * @brief RAII handle to a platform window. Will automatically close
     * the window when out of scope.
     */
    class window {
        //TYPES
    public:
        struct descriptor {
            std::string title{ "Ember Window" };
            maths::vec2i size{};
        };

        //FUNCTIONS
    public:
        inline window();

        window(window const &other) = delete;
        inline window(window &&other) noexcept;

        window &operator=(window const &other) = delete;
        inline window &operator=(window &&other) noexcept;

        inline ~window();

        virtual bool is_open() const = 0;

        virtual void close() = 0;
    };
}

namespace ember::platform {
    EMBER_API memory::unique_ptr<window> open_window(window::descriptor const &descriptor);
}

#include "window.inl"