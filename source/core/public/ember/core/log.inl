#include <fmt/format.h>

namespace ember {
    template<typename... args_t>
    void logger::log(std::string_view category, log_level level, std::string_view msg, args_t &&...args) {
        fmt::memory_buffer message_buffer{};
        fmt::format_to(message_buffer, msg, std::forward<args_t>(args)...);

        std::string_view constexpr full_message_format{ "{}: {}" };
        fmt::memory_buffer full_message_buffer{};
        fmt::format_to(full_message_buffer, full_message_format, category, std::string_view{ message_buffer.data(), message_buffer.size() });

        do_log(level, std::string_view{ full_message_buffer.data(), full_message_buffer.size() });
    }
}