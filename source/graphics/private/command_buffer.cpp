#include "ember/graphics/command_buffer.hpp"

#include <ember/memory/memory.hpp>

static std::size_t constexpr initial_buffer_size{ EMBER_KB(1) };
static std::size_t constexpr command_alignment{ 16 };

namespace ember::graphics {
    command_buffer::command_buffer() = default;
    
    command_buffer::command_buffer(command_buffer &&other) noexcept
        : buffer{ other.buffer }
        , buffer_size{ other.buffer_size }
        , pos{ other.pos } {
        other.buffer = nullptr;
    }

    command_buffer &command_buffer::operator=(command_buffer &&other) noexcept {
        buffer      = other.buffer;
        buffer_size = other.buffer_size;
        pos         = other.pos;

        other.buffer = nullptr;
        return *this;
    }

    command_buffer::~command_buffer() {
        if(buffer != nullptr) {
            memory::free(buffer);
        }
    }

    std::byte *command_buffer::alloc(std::size_t bytes) {
        if(pos + bytes > buffer_size) {
            if(buffer_size == 0) {
                buffer_size = initial_buffer_size;
                buffer      = memory::alloc(buffer_size, command_alignment);
            } else {
                buffer_size *= 2;
                buffer = memory::realloc(buffer, buffer_size, command_alignment);
            }
        }

        std::byte *const memory{ buffer + pos };
        pos += bytes;

        return memory;
    }
}