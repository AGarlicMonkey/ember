namespace ember::graphics {
    void command_buffer::reset() {
        pos = 0;
    }

    template<typename command_type, typename... args_t>
    command_type &command_buffer::alloc_command(args_t &&...args) requires std::is_trivially_destructible_v<command_type> {
        std::byte *const memory{ alloc(sizeof(command_type)) };

        command_type *const command{ new(memory) command_type{ std::forward<args_t>(args)... } };
        return *command;
    }
}