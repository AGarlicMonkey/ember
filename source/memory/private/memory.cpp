#include "ember/memory/memory.hpp"

#include "allocators/global_allocator.hpp"

namespace ember::memory {
    std::byte *alloc(std::size_t const bytes, std::size_t const alignment) {
#if !EMBER_MEMORY_DISABLE_GLOBAL_ALLOCATOR
        return global_allocator::get().alloc(bytes, alignment);
#else
        return reinterpret_cast<std::byte *>(std::malloc(bytes));
#endif
    }

    std::byte *realloc(std::byte *&original, std::size_t const bytes, std::size_t const alignment) {
#if !EMBER_MEMORY_DISABLE_GLOBAL_ALLOCATOR
        return global_allocator::get().realloc(original, bytes, alignment);
#else
        return reinterpret_cast<std::byte *>(std::realloc(original, bytes));
        original = nullptr;
#endif
    }

    void free(std::byte *&memory) {
#if !EMBER_MEMORY_DISABLE_GLOBAL_ALLOCATOR
        global_allocator::get().free(memory);
#else
        std::free(memory);
        memory = nullptr;
#endif
    }
}