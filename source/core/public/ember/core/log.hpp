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
        void log(std::string_view category, log_level level, std::string_view msg, args_t &&...args);

        void flush();

        void add_sink(std::shared_ptr<spdlog::sinks::sink> sink);

    private:
        void do_log(log_level level, std::string_view msg);
    };
}

    #include "log.inl"

    #define EBMER_LOG_ADD_FILE_SINK(name)                                                 \
        {                                                                                 \
            auto sink{ std::make_shared<spdlog::sinks::basic_file_sink_mt>(name, true) }; \
            sink->set_pattern("[%D %T][%l] %v");                                          \
            ::ember::logger::get().add_sink(std::move(sink));                             \
        }

    #define EMBER_INTERNAL_EXPAND_CATEGORY(category_name) LOG_CATEGORY##category_name
    #define EMBER_LOG_CATEGORY(category_name)                         \
        struct EMBER_INTERNAL_EXPAND_CATEGORY(category_name) {        \
            static std::string_view constexpr name{ #category_name }; \
        };

    #define EMBER_LOG(category, level, msg, ...) ::ember::logger::get().log(EMBER_INTERNAL_EXPAND_CATEGORY(category)::name, level, msg, __VA_ARGS__)

#else
    #define EBMER_LOG_ADD_FILE_SINK(name)
    #define EMBER_LOG_CATEGORY(category_name)
    #define EMBER_LOG(level, msg, ...)
#endif

#ifdef _MSC_VER
    #define EMBER_FUNCTION_NAME __FUNCSIG__
#else
    #define EMBER_FUNCTION_NAME __PRETTY_FUNCTION__
#endif

#if EMBER_CORE_ENABLE_ASSERTIONS
    #ifdef _MSC_VER
        #define EMBER_DEBUG_BREAK __debugbreak()
    #else
        #error ember currently only supported on MSVC
    #endif

EMBER_LOG_CATEGORY(EmberAssertion)

    #define EMBER_CHECK(expression)                                                                                 \
        {                                                                                                           \
            if(!(expression)) {                                                                                     \
                EMBER_LOG(EmberAssertion, ::ember::log_level::critical, "Assertion failed: {0}", #expression);      \
                EMBER_LOG(EmberAssertion, ::ember::log_level::critical, "\tfunction: {0}", EMBER_FUNCTION_NAME); \
                EMBER_DEBUG_BREAK;                                                                                  \
            }                                                                                                       \
        }
    #define EMBER_CHECK_MSG(expression, msg)                                                                       \
        {                                                                                                          \
            if(!(expression)) {                                                                                    \
                EMBER_LOG(EmberAssertion, ::ember::log_level::critical, "Assertion failed: {0}", #expression);     \
                EMBER_LOG(EmberAssertion, ::ember::log_level::critical, "\tmessage: {0}", msg);                  \
                EMBER_LOG(EmberAssertion, ::ember::log_level::critical, "\tfunction: {0}", EMBER_FUNCTION_NAME); \
                EMBER_DEBUG_BREAK;                                                                                 \
            }                                                                                                      \
        }

    #define EMBER_VERIFY(expression) EMBER_CHECK(expression)
    #define EMBER_VERIFY_MSG(expression, msg) EMBER_CHECK_MSG(expression, msg)
#else
    #define EMBER_CHECK(expression)
    #define EMBER_CHECK_MSG(expression, msg)

    #define EMBER_VERIFY(expression) expression
    #define EMBER_VERIFY_MSG(expression, msg) expression
#endif