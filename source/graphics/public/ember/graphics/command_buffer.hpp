#pragma once

#include <concepts>
#include <cstddef>

namespace ember::graphics {
    class command_buffer {
        //VARIABLES
    private:
        std::byte *buffer{ nullptr };
        std::size_t buffer_size{ 0 };

        std::size_t pos{ 0 }; /**< Current position from the start of the command buffer. */

        //FUNCTIONS
    public:
        command_buffer();

        command_buffer(command_buffer const &other) = delete;
        command_buffer(command_buffer &&other) noexcept;

        command_buffer &operator=(command_buffer const &other) = delete;
        command_buffer &operator=(command_buffer &&other) noexcept;

        ~command_buffer();

        /**
         * @brief Reset this command buffer. Removing all currently recorded commands.
         */
        inline void reset();

    protected:
        template<typename command_type, typename... args_t>
        command_type &alloc_command(args_t &&...args) requires std::is_trivially_destructible_v<command_type>;

    private: 
        std::byte *alloc(std::size_t bytes);
    };
}

#include "command_buffer.inl"