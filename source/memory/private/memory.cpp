#include "ember/memory/memory.hpp"

#include "allocators/global_allocator.hpp"
#include "ember/memory/allocators/list_allocator.hpp"

namespace ember::memory {
    class handle_manager {
    public:
        static handle create_handle(std::byte *memory) {
            return { memory };
        }

        static void reset_handle(handle &handle) {
            handle.memory = nullptr;
        }
    };
}

namespace ember::memory {
    handle alloc(std::size_t bytes, std::size_t alignment) {
        return handle_manager::create_handle(global_allocator::get().alloc(bytes, alignment));
    }

    void free(handle &handle) {
        global_allocator::get().free(handle.get());
        handle_manager::reset_handle(handle);
    }
}