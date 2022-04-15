#pragma once

#include <ember/core/exception.hpp>

namespace ember::graphics {
    class graphics_exception : public exception {
    public:
        using exception::exception;
    };

    /**
     * @brief Thrown if calling an API function has failed verification.
     */
    class api_verification_exception : public graphics_exception {
    public:
        using graphics_exception::graphics_exception;
    };

    /**
     * @brief Thrown if device selection has failed.
     */
    class device_selection_failed_exception : public graphics_exception {
    public:
        using graphics_exception::graphics_exception;
    };

    /**
     * @brief Thrown if the graphics queue for a device does not support presenting a swapchain.
     */
    class present_not_available_exception : public graphics_exception {
    public:
        using graphics_exception::graphics_exception;
    };
}