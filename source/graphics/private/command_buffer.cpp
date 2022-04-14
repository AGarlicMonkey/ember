#include "ember/graphics/command_buffer.hpp"

#include <ember/memory/memory.hpp>

static std::size_t constexpr initial_buffer_size{ EMBER_KB(1) };
static std::size_t constexpr command_alignment{ 16 };

namespace ember::graphics {
    command_buffer::command_buffer() = default;

    command_buffer::command_buffer(command_buffer &&other) noexcept
        : buffer_memory{ other.buffer_memory }
        , buffer_size{ other.buffer_size }
        , pos{ other.pos } {
        other.buffer_memory = nullptr;
    }

    command_buffer &command_buffer::operator=(command_buffer &&other) noexcept {
        buffer_memory = other.buffer_memory;
        buffer_size   = other.buffer_size;
        pos           = other.pos;

        other.buffer_memory = nullptr;

        return *this;
    }

    command_buffer::~command_buffer() {
        if(buffer_memory != nullptr) {
            memory::free(buffer_memory);
        }
    }

    std::byte *command_buffer::alloc(std::size_t bytes) {
        if(pos + bytes > buffer_size) {
            if(buffer_size == 0) {
                buffer_size   = initial_buffer_size;
                buffer_memory = memory::alloc(buffer_size, command_alignment);
            } else {
                buffer_size *= 2;
                buffer_memory = memory::realloc(buffer_memory, buffer_size, command_alignment);
            }
        }

        std::byte *const memory{ buffer_memory + pos };
        pos += bytes;

        return memory;
    }
}