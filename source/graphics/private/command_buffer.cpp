#include "ember/graphics/command_buffer.hpp"

#include "commands.hpp"

#include <ember/memory/memory.hpp>

static std::size_t constexpr initial_buffer_size{ EMBER_KB(1) };
static std::size_t constexpr command_alignment{ 0 };//Commands are tightly packed

namespace ember::graphics {
    std::pair<command_type, std::byte *> command_buffer::iterator::operator*() const {
        command_type const type{ *reinterpret_cast<command_type const *>(ptr - sizeof(command_type)) };
        return { type, ptr };
    }

    command_buffer::iterator &command_buffer::iterator::operator++() {
        ptr = reinterpret_cast<std::byte *>(reinterpret_cast<command *>(ptr)->next);
        return *this;
    }

    command_buffer::command_buffer(command_buffer &&other) noexcept
        : head{ other.head }
        , arenas{ std::move(other.arenas) }
        , current_arena{ other.current_arena } {
    }

    command_buffer &command_buffer::operator=(command_buffer &&other) noexcept {
        head          = other.head;
        arenas        = std::move(other.arenas);
        current_arena = other.current_arena;

        return *this;
    }

    command_buffer::~command_buffer() {
        if(!arenas.empty()) {
            destruct_items();
            for(auto &arena : arenas) {
                memory::free(arena.memory);
            }
        }
    }

    std::byte *command_buffer::alloc(std::size_t bytes) {
        if(current_arena == nullptr || current_arena->pos + bytes > current_arena->size) {
            arena *next_arena{ nullptr };

            for(auto &arena : arenas) {
                if(arena.pos == 0) {
                    next_arena = &arena;
                    break;
                }
            }

            if(next_arena == nullptr) {
                arenas.emplace_back(memory::alloc(initial_buffer_size, command_alignment), initial_buffer_size);
                next_arena = &arenas.back();
            }

            current_arena = next_arena;
        }

        std::byte *const memory{ current_arena->memory + current_arena->pos };
        current_arena->pos += bytes;

        return memory;
    }

    void command_buffer::destruct_items() {
        for(auto &&[type, memory] : *this) {
            destruct_command(type, memory);
        }
        for(auto &arena : arenas) {
            arena.pos = 0;
        }
    }
}