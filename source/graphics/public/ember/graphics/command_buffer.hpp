#pragma once

#include "ember/graphics/command_buffer_iterator.hpp"
#include "ember/graphics/command_types.hpp"

#include <cstddef>
#include <ember/containers/array.hpp>

namespace ember::graphics {
    class command_buffer {
        //TYPES
    private:
        //TODO: Would like everything in a single memory block but it gets complicated moving / copying commands
        //so for now we'll allocate different arenas.
        struct arena {
            std::byte *memory{ nullptr };
            std::size_t size{ 0 };
            std::size_t pos{ 0 };
        };

        //VARIABLES
    private:
        command *head{ nullptr };
        command *current{ nullptr };
        containers::array<arena> arenas{};
        arena *current_arena{ nullptr };

        //FUNCTIONS
    public:
        inline command_buffer();

        command_buffer(command_buffer const &other) = delete;
        command_buffer(command_buffer &&other) noexcept;

        command_buffer &operator=(command_buffer const &other) = delete;
        command_buffer &operator=(command_buffer &&other) noexcept;

        virtual ~command_buffer();

        /**
         * @brief Reset this command buffer. Removing all currently recorded commands.
         */
        inline void reset();

        /**
         * @brief Returns an iterator to the beginning of this command buffer.
         * @return 
         */
        inline command_buffer_iterator begin() const;
        /**
         * @brief Returns an iterator to the end of this command buffer.
         * @return 
         */
        inline command_buffer_iterator end() const;

    protected:
        /**
         * @brief Record a command into this buffer.
         * @tparam type Type of command to record.
         * @tparam args_type 
         * @param args Construction args for the command.
         */
        template<command_type type, typename... args_type>
        void record_command(args_type &&...args);

    private:
        std::byte *alloc(std::size_t bytes);

        void destruct_items();
    };
}

#include "command_buffer.inl"