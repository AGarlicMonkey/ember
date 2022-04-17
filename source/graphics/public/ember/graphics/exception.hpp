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

    /**
     * @brief Thrown if there was a problem creating a resource.
     */
    class resource_creation_exception : public graphics_exception {
        using graphics_exception::graphics_exception;
    };

    /**
     * @brief Thrown if a shader fails to compile
     */
    class shader_compilation_failed_exception : public graphics_exception {
    public:
        std::string shader_name{};

        explicit shader_compilation_failed_exception(std::string const &message, std::string shader_name)
            : graphics_exception{ message }
            , shader_name{ std::move(shader_name) } {
        }
        explicit shader_compilation_failed_exception(char const *message, std::string shader_name)
            : graphics_exception{ message }
            , shader_name{ std::move(shader_name) } {
        }
    };
}