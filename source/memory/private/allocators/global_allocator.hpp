#pragma once

#include <cstddef>
#include <ember/core/export.hpp>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace ember::memory {
    /**
     * @brief global_allocator is essentially a free list allocator except it bypasses
     * ember's memory/allocation system. Providing a place to allocate memory from for the entire application.
     */
    class global_allocator {
        //TYPES
    private:
        struct block {
            block *next{ nullptr };
            block *prev{ nullptr };

            bool is_free{ true };

            std::size_t arena_index{ 0 }; /**< Which arena this block belongs to. */

            std::size_t block_offset{ 0 }; /**< Offset into the arena memory for the whole block. */
            std::size_t data_offset{ 0 };  /**< Offset into the arena memory containing the block data. Respects the data's alignment. */
            std::size_t size{ 0 };         /**< Size of the entire block of memory. */
        };

        struct arena {
            std::byte *memory{ nullptr };
            std::size_t size{ 0 };

            std::vector<block *> free_list{};
        };

        //VARIABLES
    private:
        std::size_t size{ 0 }; /**< Total size of this allocator (sum of all arenas) */
        std::vector<arena> memory_arenas{};

        std::recursive_mutex allocator_mutex{};

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

        std::byte *alloc(std::size_t const bytes, std::size_t const alignment);
        std::byte *realloc(std::byte *&original, std::size_t const bytes, std::size_t const alignment);
        void free(std::byte *&memory);

        inline std::size_t get_size() const;

    private:
        void return_block_to_freelist(block *const block);

        block *get_block_from_memory(std::byte const *const memory);

        block *create_new_block(std::size_t const arena_index, std::size_t const offset, std::size_t const bytes);
        void create_new_arena(std::size_t const bytes);

        void remove_block_from_free_list(std::vector<block *> &free_list, block *const block);
    };
}

#include "global_allocator.inl"