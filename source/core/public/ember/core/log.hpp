#pragma once

#if EMBER_CORE_ENABLE_LOGGING
    #include "ember/core/export.hpp"

    #include <fmt/format.h>
    #include <memory>
    #include <string_view>

namespace spdlog {
    class logger;
    namespace sinks {
        class sink;
    }
}

namespace ember {
    enum class log_level {
        trace,
        debug,
        info,
        warn,
        error,
        critical,
    };
}

namespace ember {
    class EMBER_API logger {
        //VARIABLES
    private:
        std::unique_ptr<spdlog::logger> internal_logger{ nullptr };

        //FUNCTIONS
    public:
        logger();

        logger(logger const &other)     = delete;
        logger(logger &&other) noexcept = delete;

        logger &operator=(logger const &other) = delete;
        logger &operator=(logger &&other) = delete;

        ~logger();

        static logger &get();

        template<typename... args_t>
        void log(log_level level, std::string_view msg, args_t &&...args);

        void add_sink(std::shared_ptr<spdlog::sinks::sink> sink);

    private:
        void do_log(log_level level, std::string_view msg);
    };
}

    #include "log.inl"

    #define EMBER_LOG(level, msg, ...) ::ember::logger::get().log(level, msg, __VA_ARGS__)

    #define EBMER_LOG_ADD_SINK(sink) ::ember::logger::get().add_sink(std::move(sink))
#else
    #define EMBER_LOG(level, msg, ...)

    #define EBMER_LOG_ADD_SINK(sink)
#endif

#if EMBER_CORE_ENABLE_ASSERTIONS
    #define EMBER_CHECK(x) x //TODO
    #define EMBER_VERIFY(x) x//TODO
#else
    #define EMBER_CHECK(x)
    #define EMBER_VERIFY(x) x
#endif

#if EMBER_CORE_ENABLE_EXCEPTIONS
    #define EMBER_THROW(exception) throw exception
#else
    #define EMBER_THROW(exception)
#endif