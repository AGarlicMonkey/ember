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
#else
    #define EMBER_THROW(exception) std::abort()
#endif

#define EMBER_THROW_IF_FAILED(condition, exception) \
    if(!(condition)) {                              \
        EMBER_THROW(exception);                     \
    }