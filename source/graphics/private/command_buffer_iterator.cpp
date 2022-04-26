#include "ember/graphics/command_buffer_iterator.hpp"

#include "commands.hpp"

namespace ember::graphics {
    std::pair<command_type, std::byte *> command_buffer_iterator::operator*() const {
        command_type const type{ *reinterpret_cast<command_type const *>(ptr - sizeof(command_type)) };
        return { type, ptr };
    }

    command_buffer_iterator &command_buffer_iterator::operator++() {
        ptr = reinterpret_cast<std::byte *>(reinterpret_cast<command *>(ptr)->next);
        return *this;
    }
}