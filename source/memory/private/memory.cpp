#include "ember/memory/memory.hpp"

#include "ember/memory/allocators/list_allocator.hpp"
#include "allocators/global_allocator.hpp"

namespace ember::memory {
    handle alloc(std::size_t bytes, std::size_t alignment) {
        return { global_allocator::get().alloc(bytes, alignment) };
    }

    void free(handle &handle) {
        //TODO:
    }
}