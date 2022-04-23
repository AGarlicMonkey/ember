namespace ember::graphics {
    command_buffer_iterator::~command_buffer_iterator() = default;

    bool operator==(command_buffer_iterator const &lhs, command_buffer_iterator const &rhs) {
        return lhs.ptr == rhs.ptr;
    }

    bool operator!=(command_buffer_iterator const &lhs, command_buffer_iterator const &rhs) {
        return !(lhs == rhs);
    }

    command_buffer_iterator::command_buffer_iterator(std::byte *ptr, std::size_t max)
        : ptr{ ptr }
        , max{ max } {
    }
}