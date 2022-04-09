#include "ember/memory/memory.hpp"

#include "allocators/global_allocator.hpp"

namespace ember::memory {
    std::byte *alloc(std::size_t bytes, std::size_t alignment) {
        return global_allocator::get().alloc(bytes, alignment);
    }

    void free(std::byte *&memory) {
        global_allocator::get().free(memory);
    }
}