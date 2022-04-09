#pragma once

#include "ember/memory/handle.hpp"

#include <ember/core/export.hpp>

#define EMBER_KB(x) (x * 1024)
#define EMBER_MB(x) (EMBER_KB(x) * 1024)
#define EMBER_GB(x) (EMBER_MB(x) * 1024)

namespace ember::memory {
    EMBER_API handle alloc(std::size_t bytes, std::size_t alignment);
    EMBER_API void free(handle &handle);
}