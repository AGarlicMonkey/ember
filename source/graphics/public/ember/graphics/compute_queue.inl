namespace ember::graphics {
    compute_queue::compute_queue() = default;

    compute_queue::compute_queue(compute_queue &&other) noexcept = default;

    compute_queue &compute_queue::operator=(compute_queue &&other) noexcept = default;

    compute_queue::~compute_queue() = default;

    compute_command_buffer compute_queue::allocate_command_buffer() {
        //TODO:
        return {};
    }
}