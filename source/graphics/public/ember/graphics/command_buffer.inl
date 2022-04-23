namespace ember::graphics {
    command_buffer::command_buffer() = default;

    void command_buffer::reset() {
        //TODO: Call destructors

        pos = 0;
    }

    command_buffer_iterator command_buffer::begin() const {
        return { buffer_memory, pos };
    }

    command_buffer_iterator command_buffer::end() const {
        return { buffer_memory + pos, pos };
    }

    template<command_type type, typename... args_type>
    void command_buffer::record_command(args_type &&...args) {
        std::size_t constexpr type_size{ sizeof(command_type) };
        std::size_t constexpr command_size{ sizeof(recorded_command<type>) };

        std::byte *memory{ alloc(type_size + command_size) };

        new(memory) command_type{ type };
        memory += type_size;
        new(memory) recorded_command<type>{ std::forward<args_type>(args)... };
    }
}