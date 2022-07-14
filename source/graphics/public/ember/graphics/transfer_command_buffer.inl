namespace ember::inline graphics {
    transfer_command_buffer::transfer_command_buffer() = default;

    transfer_command_buffer::transfer_command_buffer(transfer_command_buffer &&other) noexcept = default;

    transfer_command_buffer &transfer_command_buffer::operator=(transfer_command_buffer &&other) noexcept = default;

    transfer_command_buffer::~transfer_command_buffer() = default;
}