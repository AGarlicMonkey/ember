#pragma once

#include "ember/graphics/command_types.hpp"

#include <cstddef>
#include <utility>

namespace ember::graphics {
    class command_buffer_iterator {
        friend class command_buffer;
        //VARIABLES
    private:
        std::byte *ptr{ nullptr };

        //FUNCTIONS
    public:
        command_buffer_iterator() = delete;

        command_buffer_iterator(command_buffer_iterator const &other)     = delete;
        command_buffer_iterator(command_buffer_iterator &&other) noexcept = delete;

        command_buffer_iterator &operator=(command_buffer_iterator const &other) = delete;
        command_buffer_iterator &operator=(command_buffer_iterator &&other) noexcept = delete;

        inline ~command_buffer_iterator();

        std::pair<command_type, std::byte *> operator*() const;

        command_buffer_iterator &operator++();

        inline friend bool operator==(command_buffer_iterator const &lhs, command_buffer_iterator const &rhs);
        inline friend bool operator!=(command_buffer_iterator const &lhs, command_buffer_iterator const &rhs);

    private:
        inline command_buffer_iterator(std::byte *ptr);
    };
}

#include "command_buffer_iterator.inl"