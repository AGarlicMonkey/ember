#pragma once

#include "ember/graphics/command_types.hpp"

#include <cstddef>
#include <ember/containers/array.hpp>
#include <ember/core/export.hpp>

namespace ember::inline graphics {
    class EMBER_API command_buffer {
        //TYPES
    public:
        class iterator {
            friend command_buffer;

            //VARIABLES
        private:
            std::byte *ptr{ nullptr };

            //FUNCTIONS
        public:
            iterator() = delete;

            iterator(iterator const &other)     = delete;
            iterator(iterator &&other) noexcept = delete;

            iterator &operator=(iterator const &other)     = delete;
            iterator &operator=(iterator &&other) noexcept = delete;

            inline ~iterator();

            std::pair<command_type, std::byte *> operator*() const;

            iterator &operator++();

            inline friend bool operator==(iterator const &lhs, iterator const &rhs);
            inline friend bool operator!=(iterator const &lhs, iterator const &rhs);

        private:
            inline iterator(std::byte *ptr);
        };

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
        array<arena> arenas{};
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
        inline iterator begin() const;
        /**
         * @brief Returns an iterator to the end of this command buffer.
         * @return 
         */
        inline iterator end() const;

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