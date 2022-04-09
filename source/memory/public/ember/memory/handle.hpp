#pragma once

#include <cstddef>
#include <ember/core/export.hpp>

namespace ember::memory {
    /**
     * @brief 
     */
    class handle {
        friend class handle_manager;
        
        //VARIABLES
    private:
        std::byte *memory{ nullptr };

        //FUNCTIONS
    public:
        handle() = default;

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

    private:
        handle(std::byte *memory)
            : memory{ memory } {
        }
    };
}

#include "handle.inl"