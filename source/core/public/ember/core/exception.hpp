#pragma once

#include <stdexcept>
#include <string>

namespace ember {
    class exception : public std::runtime_error {
    public:
        explicit exception(std::string const &message)
            : std::runtime_error(message) {}
        explicit exception(char const *message)
            : std::runtime_error(message) {}
    };
}

#if EMBER_CORE_ENABLE_EXCEPTIONS
    #define EMBER_THROW(exception) throw exception
    #define EMBER_THROW_IF_FAILED(condition, exception) \
        if(!(condition)) {                              \
            throw exception;                            \
        }
#else
    #define EMBER_THROW(exception)
    #define EMBER_THROW_IF_FAILED(condition, exception)
#endif