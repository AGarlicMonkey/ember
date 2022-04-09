#pragma once

#include <cstddef>
#include <ember/core/export.hpp>
#include <list>
#include <mutex>

namespace ember::memory {
    /**
     * @brief global_allocator is essentially a free list allocator except it bypasses
     * ember's memory/allocation system. Providing a place to allocate memory from for the entire application.
     */
    class global_allocator {
        //TYPES
    private:
        struct block {
            bool is_free{ true };
            std::size_t offset{ 0 };         /**< Offset into the backing memory. */
            std::size_t aligned_offset{ 0 }; /**< The offset into the backing memory that respects the allocation's alignment. */
            std::size_t size{ 0 };           /**< Size of the entire block of memory. */
        };

        //VARIABLES
    private:
        std::byte *backing_memory{ nullptr };
        std::size_t size{ 0 };
        std::list<block> free_list{};//Keep track of the free list outside of the backing memory to be able to utilise the entire range.

        std::mutex allocation_mutex{};

        //FUNCTIONS
    public:
        global_allocator() = delete;
        global_allocator(std::size_t initial_memory_size);

        global_allocator(global_allocator const &other)     = delete;
        global_allocator(global_allocator &&other) noexcept = delete;

        global_allocator &operator=(global_allocator const &other) = delete;
        global_allocator &operator=(global_allocator &&other) noexcept = delete;

        ~global_allocator() = default;

        static global_allocator &get();

        std::byte *alloc(std::size_t bytes, std::size_t alignment);
        void free(std::byte *&memory);
    };
}