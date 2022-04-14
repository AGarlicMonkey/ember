namespace ember::graphics {
    compute_command_buffer::compute_command_buffer() = default;

    compute_command_buffer::compute_command_buffer(compute_command_buffer &&other) noexcept = default;

    compute_command_buffer &compute_command_buffer::operator=(compute_command_buffer &&other) noexcept = default;

    compute_command_buffer::~compute_command_buffer() = default;
}