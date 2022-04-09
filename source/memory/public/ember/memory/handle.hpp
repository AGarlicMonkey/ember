#pragma once

#include <cstddef>
#include <ember/core/export.hpp>

namespace ember::memory {
    /**
     * @brief 
     */
    class EMBER_API handle {
        //VARIABLES
    private:
        std::byte *memory{ nullptr };

        //FUNCTIONS
    public:
        handle() = default;
        handle(std::byte *memory)
            : memory{ memory } {
        }

        handle(handle const &other)     = delete;
        handle(handle &&other) noexcept = default;

        handle &operator=(handle const &other) = delete;
        handle &operator=(handle &&other) noexcept = default;

        ~handle() = default;

        inline bool is_valid() const {
            return memory != nullptr;
        };

        inline std::byte *get() {
            return memory;
        };
    };
}

#include "handle.inl"