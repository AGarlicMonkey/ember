namespace ember::graphics {
    graphics_command_buffer::graphics_command_buffer() = default;

    graphics_command_buffer::graphics_command_buffer(graphics_command_buffer &&other) noexcept = default;

    graphics_command_buffer &graphics_command_buffer::operator=(graphics_command_buffer &&other) noexcept = default;

    graphics_command_buffer::~graphics_command_buffer() = default;
}