#pragma once

#include <ember/core/exception.hpp>

namespace ember::inline memory {
    class memory_exception : public exception {
    public:
        using exception::exception;
    };
}