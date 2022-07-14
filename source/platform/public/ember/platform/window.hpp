#pragma once

#include "ember/platform/input.hpp"

#include <ember/core/export.hpp>
#include <ember/maths/vector.hpp>
#include <ember/memory/unique_ptr.hpp>
#include <optional>
#include <string>
#include <variant>

namespace ember::inline platform {
    /**
     * @brief RAII handle to a platform window. Will automatically close
     * the window when out of scope.
     */
    class window {
        //TYPES
    public:
        struct descriptor {
            std::string title{ "Ember Window" };
            vec2u size{ 800, 600 };
        };

        struct mouse_event {
            enum class type {
                invalid,

                move,
                pressed,
                released,
                wheel_up,
                wheel_down,
                enter,
                leave,
            };

            type type{ type::invalid };

            mouse_button button{ mouse_button::none };
            vec2i pos{ 0, 0 };
        };

        struct key_event {
            enum class type {
                invalid,

                pressed,
                released,
            };

            type type{ type::invalid };

            key key{ key::none };
        };

        struct char_event {
            char character;
        };

        using event = std::variant<mouse_event, key_event, char_event>;

        //FUNCTIONS
    public:
        inline window();

        window(window const &other) = delete;
        inline window(window &&other) noexcept;

        window &operator=(window const &other) = delete;
        inline window &operator                =(window &&other) noexcept;

        inline ~window();

        /**
         * @brief Pumps the message queue for this window. Returning the most recent event.
         * @return 
         */
        virtual std::optional<event> get_event() = 0;

        virtual bool is_open() const = 0;
        virtual void close()         = 0;

        /**
         * @brief Returns the size of the window's client region.
         * @return 
         */
        virtual vec2u get_size() const = 0;

        /**
         * @brief Returns the platform's native window handle.
         * @return 
         */
        virtual void *get_native_window() const = 0;
    };
}

namespace ember::platform {
    /**
     * @brief Opens a new window.
     * @param descriptor 
     * @return 
     */
    EMBER_API unique_ptr<window> open_window(window::descriptor const &descriptor);
}

#include "window.inl"