namespace ember::platform {
    window::window() = default;

    window::window(window &&other) noexcept = default;

    window &window::operator=(window &&other) noexcept = default;

    window::~window() = default;
}