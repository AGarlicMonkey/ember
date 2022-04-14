namespace ember::graphics {
    graphics_queue::graphics_queue() = default;

    graphics_queue::graphics_queue(graphics_queue &&other) noexcept = default;

    graphics_queue &graphics_queue::operator=(graphics_queue &&other) noexcept = default;

    graphics_queue::~graphics_queue() = default;

    transfer_command_buffer graphics_queue::allocate_command_buffer() {
        //TODO:
        return {};
    }
}