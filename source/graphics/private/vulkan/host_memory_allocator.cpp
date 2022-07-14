#include "host_memory_allocator.hpp"

#include <ember/memory/memory.hpp>
#include <cstring>

namespace ember::inline graphics {
    void *alloc(void *user_data, std::size_t size, std::size_t alignment, VkSystemAllocationScope allocation_scope) {
        return memory::alloc(size, alignment);
    }

    void *realloc(void *user_data, void *original, std::size_t size, std::size_t alignment, VkSystemAllocationScope allocation_scope) {
        auto *original_memory{ reinterpret_cast<std::byte *>(original) };
        return memory::realloc(original_memory, size, alignment);
    }

    void free(void *user_data, void *memory) {
        auto *mem{ reinterpret_cast<std::byte *>(memory) };
        memory::free(mem);
    }
}