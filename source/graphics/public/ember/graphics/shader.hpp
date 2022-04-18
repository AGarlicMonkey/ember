#pragma once

#include <cinttypes>
#include <ember/core/enum.hpp>

namespace ember::graphics {
    /**
     * @brief 
     */
    class shader {
        //TYPES
    public:
        using stage_type = std::uint8_t;
        enum class stage {
            vertex  = 1 << 0,
            pixel   = 1 << 1,
            compute = 1 << 2,
        };

        //FUNCTIONS
    public:
        virtual ~shader() = default;
    };

    EMBER_ENUM_BIT_FLAG_OPERATORS(shader::stage, shader::stage_type);
}
