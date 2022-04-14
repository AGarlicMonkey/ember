namespace ember::graphics {
    transfer_queue::transfer_queue() = default;

    transfer_queue::transfer_queue(transfer_queue &&other) noexcept = default;

    transfer_queue &transfer_queue::operator=(transfer_queue &&other) noexcept = default;

    transfer_queue::~transfer_queue() = default;

    transfer_command_buffer transfer_queue::allocate_command_buffer() {
        //TODO:
        return {};
    }
}