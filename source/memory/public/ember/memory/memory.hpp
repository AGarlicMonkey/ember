#pragma once

#include <cinttypes>
#include <cstddef>
#include <ember/core/export.hpp>

#define EMBER_KB(x) (x * 1024u)
#define EMBER_MB(x) (EMBER_KB(x) * 1024u)
#define EMBER_GB(x) (EMBER_MB(x) * 1024u)

namespace ember::memory {
    /**
     * @brief Allocates memory from the global memory pool.
     * @param bytes How many bytes to allocate.
     * @param alignment How to align the allocation.
     * @return A pointer to the newly allocated memory.
     */
    EMBER_API std::byte *alloc(std::size_t const bytes, std::size_t const alignment);
    /**
     * @brief Puts allocated memory back into the global memory pool.
     * @param memory Pointer to the memory to return.
     * @return 
     */
    EMBER_API void free(std::byte *&memory);

    /**
     * @brief Returns the current size in bytes on the global memory pool.
     * @return 
     */
    EMBER_API std::size_t get_global_pool_size();
}