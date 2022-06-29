#pragma once

#include "ember/graphics/shader.hpp"

#include <cstddef>

namespace ember::graphics {
    struct push_constant_descriptor {
        shader::stage stage{ 0 };
        std::size_t offset{ 0 }; /**< Offset specified in the shader (layout(offset = x)). Required if using different push constants for different stages. */
        std::size_t bytes{ 0 };
    };
}