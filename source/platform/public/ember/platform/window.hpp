#pragma once

#include <ember/core/export.hpp>

namespace ember::platform {
    struct window_handle {
        void *platform_handle{ nullptr };
    };
}

namespace ember::platform {
    EMBER_API window_handle open_window();
    EMBER_API void close_window(window_handle &handle);

    EMBER_API bool is_window_open(window_handle const &handle);
}