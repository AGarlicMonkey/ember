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
        /**
         * @brief Contains data about a block of memory within a memory arena.
         */
        struct block_header {
            block_header *next{ nullptr }; /**< Next block within the arena. Can be nullptr.*/
            block_header *prev{ nullptr }; /**< Previous block within the arena. Can be nullptr.*/

            bool is_free{ true };

            std::size_t arena_index{ 0 }; /**< Which arena this block belongs to. */

            std::uint8_t padding{ 0 }; /**< How many bytes this header was aligned by to accomodate an allocation.*/
            std::size_t offset{ 0 };   /**< Offset into the arena memory for the whole block (including this header). */
            std::size_t size{ 0 };     /**< Size of the entire block of memory (excluding the header). */
        };

        struct arena {
            std::byte *memory{ nullptr };
            std::size_t size{ 0 }; /**< Total size of the memroy arena. */
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
        void apply_padding_to_header(block_header *&header, std::uint8_t const padding);
        void reset_header_padding(block_header *&header);

        block_header *get_header_from_memory(std::byte *const memory);

        block_header *create_new_block(std::size_t const arena, std::size_t const offset, std::size_t const bytes);
        void return_block_to_freelist(block_header *const block);

        void create_new_arena(std::size_t const bytes);
    };
}

#include "global_allocator.inl"