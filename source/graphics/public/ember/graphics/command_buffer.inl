namespace ember::inline graphics {
    command_buffer::iterator::~iterator() = default;

    bool operator==(command_buffer::iterator const &lhs, command_buffer::iterator const &rhs) {
        return lhs.ptr == rhs.ptr;
    }

    bool operator!=(command_buffer::iterator const &lhs, command_buffer::iterator const &rhs) {
        return !(lhs == rhs);
    }

    command_buffer::iterator::iterator(std::byte *ptr)
        : ptr{ ptr } {
    }

    command_buffer::command_buffer() = default;

    void command_buffer::reset() {
        destruct_items();
        head          = nullptr;
        current_arena = nullptr;
    }

    command_buffer::iterator command_buffer::begin() const {
        return { reinterpret_cast<std::byte *>(head) };
    }

    command_buffer::iterator command_buffer::end() const {
        return { nullptr };
    }

    template<command_type type, typename... args_type>
    void command_buffer::record_command(args_type &&...args) {
        std::size_t constexpr type_size{ sizeof(command_type) };
        std::size_t constexpr command_size{ sizeof(recorded_command<type>) };

        std::byte *memory{ alloc(type_size + command_size) };

        new(memory) command_type{ type };
        memory += type_size;
        auto *command{ new(memory) recorded_command<type>{ std::forward<args_type>(args)... } };

        if(current != nullptr) {
            current->next = command;
            current       = current->next;
        } else {
            head = current = command;
        }
    }
}