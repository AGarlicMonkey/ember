#include <fmt/format.h>

namespace ember {
    template<typename... args_t>
    void logger::log(log_level level, std::string_view msg, args_t &&...args) {
        fmt::basic_memory_buffer<char, 250> message_buffer{};
        fmt::format_to(message_buffer, msg, std::forward<args_t>(args)...);

        do_log(level, std::string_view{ message_buffer.data(), message_buffer.size() });
    }
}