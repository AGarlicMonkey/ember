#include "ember/memory/memory.hpp"

#include "allocators/global_allocator.hpp"

namespace ember::memory {
    std::byte *alloc(std::size_t const bytes, std::size_t const alignment) {
        return global_allocator::get().alloc(bytes, alignment);
    }

    std::byte *realloc(std::byte *&original, std::size_t const bytes, std::size_t const alignment) {
        return global_allocator::get().realloc(original, bytes, alignment);
    }

    void free(std::byte *&memory) {
        global_allocator::get().free(memory);
    }

    std::size_t get_global_pool_size() {
        return global_allocator::get().get_size();
    }
}