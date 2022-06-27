#include <format>

namespace ember {
    template<typename... args_t>
    void logger::log(std::string_view category, log_level level, std::string_view msg, args_t &&...args) {
        //Use vformat here as std::format uses _Basic_format_string which is consteval
        std::string const message{ std::vformat(msg, std::make_format_args(std::forward<args_t>(args)...)) };

        std::string_view constexpr full_message_format{ "{}: {}" };
        std::string const full_message{ std::format(full_message_format, category, message) };

        do_log(level, full_message);
    }
}