#pragma once

#include <ember/core/export.hpp>
#include <string>
#include <ember/maths/vector.hpp>

namespace ember::platform {
    struct window_descriptor{
        std::string title{ "Ember Window" };
        maths::vec2i size{};
    };

    struct window_handle {
        void *platform_handle{ nullptr };
    };
}

namespace ember::platform {
    EMBER_API window_handle open_window(window_descriptor const &descriptor);
    EMBER_API bool is_window_open(window_handle const &handle);
    EMBER_API void close_window(window_handle &handle);
}