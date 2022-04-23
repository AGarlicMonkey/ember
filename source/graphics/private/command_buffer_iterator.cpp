#include "ember/graphics/command_buffer_iterator.hpp"

#include "commands.hpp"

namespace ember::graphics {
    std::pair<command_type, std::byte *> command_buffer_iterator::operator*() const {
        command_type const type{ *reinterpret_cast<command_type const *>(ptr) };
        return { type, ptr + sizeof(command_type) };
    }

    command_buffer_iterator &command_buffer_iterator::operator++() {
        command_type const type{ *reinterpret_cast<command_type const *>(ptr) };
        ptr += sizeof(command_type) + get_size_of_command(type);
        return *this;
    }
}