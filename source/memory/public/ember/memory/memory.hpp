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
     * @brief Constructs a new object of type object_t from the global memory pool,
     * calling it's constructor with arg_t.
     * @param args
     * @tparam object_t 
     * @return 
     */
    template<typename object_t, typename... arg_t>
    object_t *construct(arg_t &&...args);
    /**
     * @brief Destructs an object of type object_t. Freeing it's memory and calling it's
     * destructor.
     * @tparam object_t 
     * @param object 
     */
    template<typename object_t>
    void destruct(object_t *&object);

    /**
     * @brief Returns the current size in bytes on the global memory pool.
     * @return 
     */
    EMBER_API std::size_t get_global_pool_size();
}

#include "memory.inl"