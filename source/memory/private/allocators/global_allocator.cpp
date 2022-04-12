#include "allocators/global_allocator.hpp"

#include "ember/memory/memory.hpp"

#include <ember/core/exception.hpp>
#include <ember/core/log.hpp>

EMBER_LOG_CATEGORY(EmberGlobalAllocator)

#if EMBER_CORE_ENABLE_ASSERTIONS
    #define VALIDATE_HEADER(header)                                                              \
        {                                                                                        \
            EMBER_CHECK(header->next != header);                                                 \
            EMBER_CHECK(header->prev != header);                                                 \
            EMBER_CHECK(header->size != 0);                                                      \
            EMBER_CHECK(header->arena_index < memory_arenas.size());                             \
            EMBER_CHECK(header->size <= memory_arenas[header->arena_index].size)                 \
            if(header->next != nullptr) {                                                        \
                EMBER_CHECK(header->next->prev == header);                                       \
                EMBER_CHECK(header->next->size != 0);                                            \
                EMBER_CHECK(header->next->arena_index < memory_arenas.size());                   \
                EMBER_CHECK(header->next->size <= memory_arenas[header->next->arena_index].size) \
            }                                                                                    \
            if(header->prev != nullptr) {                                                        \
                EMBER_CHECK(header->prev->next == header);                                       \
                EMBER_CHECK(header->prev->size != 0);                                            \
                EMBER_CHECK(header->prev->arena_index < memory_arenas.size());                   \
                EMBER_CHECK(header->prev->size <= memory_arenas[header->prev->arena_index].size) \
            }                                                                                    \
        }

#else
    #define VALIDATE_BLOCK(header)
#endif

static size_t constexpr arena_size{ EMBER_GB(1) };
static ::ember::memory::global_allocator *instance{ nullptr };

namespace {
    std::size_t get_remaining_alignment(std::byte const *const ptr, std::size_t required_alignment) {
        if(required_alignment == 0) {
            return true;
        }

        auto const iptr{ reinterpret_cast<std::uintptr_t>(ptr) };

        std::size_t const remaining_bytes{ iptr & required_alignment };
        std::size_t const bytes_to_offset{ remaining_bytes != 0 ? required_alignment - remaining_bytes : 0 };

        return bytes_to_offset;
    }
}

namespace ember::memory {
    global_allocator::global_allocator(std::size_t initial_memory_size)
        : size{ initial_memory_size } {
        create_new_arena(initial_memory_size);
        EMBER_LOG(EmberGlobalAllocator, log_level::info, "Initialised global memory pool with {0} bytes.", initial_memory_size);
    }

    global_allocator &global_allocator::get() {
        if(instance == nullptr) {
            instance = new global_allocator{ arena_size };
        }

        return *instance;
    }

    std::byte *global_allocator::alloc(std::size_t const bytes, std::size_t const alignment) {
        std::scoped_lock lock{ allocator_mutex };

        std::size_t const total_allocation_size{ sizeof(block_header) + bytes + alignment };//Allow enough room for the worst case aliognment
        std::byte *memory{ nullptr };

        for(std::size_t arena_index{ 0 }; arena_index < memory_arenas.size(); ++arena_index) {
            arena &arena{ memory_arenas[arena_index] };

            if(arena.size < bytes) {
                continue;
            }

            block_header *header{ reinterpret_cast<block_header *>(arena.memory) };
            while(header != nullptr) {
                if(header->is_free && header->size >= total_allocation_size) {
                    header->is_free = false;

                    //If we have space left over then insert it as a new block
                    if(size_t const remaining_space{ header->size - total_allocation_size }; remaining_space > 0) {
                        header->size = total_allocation_size;

                        size_t const new_block_offset{ header->offset + header->size };
                        block_header *const new_block{ create_new_block(arena_index, new_block_offset, remaining_space) };
                        block_header *const next_block{ header->next };

                        header->next    = new_block;
                        new_block->next = next_block;

                        if(next_block != nullptr) {
                            next_block->prev = new_block;
                        }
                        new_block->prev = header;
                    }

                    std::byte *block_memory{ arena.memory + header->offset + sizeof(block_header) };

                    std::size_t const alignment_offset{ get_remaining_alignment(block_memory, alignment) };
                    EMBER_CHECK(alignment_offset < std::numeric_limits<std::uint8_t>::max());

                    //Move the header along the chunk of memory so it is always sizeof(block_header) behind the allocation.
                    //This allows freeing blocks to be extremely quick
                    if(alignment_offset > 0) {
                        apply_padding_to_header(header, alignment_offset);
                    }

                    //Retrieve the block memory again here in case we needed to pad above.
                    memory = arena.memory + header->offset + sizeof(block_header);
                    break;
                }

                header = header->next;
            }
        }

        if(memory == nullptr) {
            std::size_t const new_arena_size{ std::max(arena_size, total_allocation_size) };
            size += new_arena_size;

            EMBER_LOG(EmberGlobalAllocator, log_level::debug, "Global memory pool filled. Allocating {0} more bytes. Current total size is {1} bytes", new_arena_size, size);

            create_new_arena(new_arena_size);

            memory = alloc(bytes, alignment);
        }

        return memory;
    }

    std::byte *global_allocator::realloc(std::byte *&original, std::size_t const bytes, std::size_t const alignment) {
        std::scoped_lock lock{ allocator_mutex };

        //TODO: Can check if next block is free and extend to avoid a copy

        block_header *original_block{ get_header_from_memory(original) };
        std::size_t const copy_size{ std::min(bytes, original_block->size) };

        std::byte *const new_alloc{ alloc(bytes, alignment) };
        std::memcpy(new_alloc, original, copy_size);

        reset_header_padding(original_block);
        return_block_to_freelist(original_block);
        original = nullptr;

        return new_alloc;
    }

    void global_allocator::free(std::byte *&memory) {
        std::scoped_lock lock{ allocator_mutex };

        if(memory == nullptr) {
            return;
        }

        block_header *header{ get_header_from_memory(memory) };
        reset_header_padding(header);
        return_block_to_freelist(header);

        memory = nullptr;
    }

    void global_allocator::apply_padding_to_header(block_header *&header, std::uint8_t const padding) {
        if(header->padding == 0) {
            return;
        }

        block_header *const next_header{ header->next };
        block_header *const prev_header{ header->prev };
        block_header header_copy{ *header };
        auto *header_memory{ reinterpret_cast<std::byte *>(header) };

        header_copy.offset += padding;
        header_copy.padding = padding;

        header_memory += padding;

        header            = reinterpret_cast<block_header *>(header_memory);
        *header           = header_copy;
        next_header->prev = header;
        prev_header->next = header;

        VALIDATE_HEADER(header);
    }

    void global_allocator::reset_header_padding(block_header *&header) {
        if(header->padding == 0) {
            return;
        }

        block_header *const next_header{ header->next };
        block_header *const prev_header{ header->prev };
        block_header header_copy{ *header };
        auto *header_memory{ reinterpret_cast<std::byte *>(header) };

        header_copy.offset -= header_copy.padding;
        header_memory -= header_copy.padding;
        header_copy.padding = 0;

        header            = reinterpret_cast<block_header *>(header_memory);
        *header           = header_copy;
        next_header->prev = header;
        prev_header->next = header;

        VALIDATE_HEADER(header);
    }

    global_allocator::block_header *global_allocator::get_header_from_memory(std::byte *const memory) {
        EMBER_THROW_IF_FAILED(memory != nullptr, exception{ "Unable to find memory inside current allocator" });
        return reinterpret_cast<block_header *>(memory - sizeof(block_header));
    }

    global_allocator::block_header *global_allocator::create_new_block(std::size_t const arena_index, std::size_t const offset, std::size_t const bytes) {
        arena &arena{ memory_arenas[arena_index] };

        block_header *const new_block{ reinterpret_cast<block_header *>(arena.memory + offset) };
        *new_block = block_header{
            .arena_index = arena_index,
            .offset      = offset,
            .size        = bytes,
        };

        EMBER_CHECK(new_block->is_free);
        VALIDATE_HEADER(new_block);

        return new_block;
    }

    void global_allocator::return_block_to_freelist(block_header *const curr_block) {
        EMBER_CHECK_MSG(curr_block->padding == 0, "headers should be reset back to the beginning of their block before returning to the free list.");

        block_header *const next_block{ curr_block->next };
        block_header *const prev_block{ curr_block->prev };
        arena &arena{ memory_arenas[curr_block->arena_index] };

        curr_block->is_free = true;

        //Attempt to merge the freed memory into either the left or right blocks or both
        if(next_block != nullptr && next_block->is_free) {
            EMBER_CHECK(next_block->prev == curr_block);

            curr_block->size += next_block->size;

            curr_block->next = next_block->next;
            if(curr_block->next != nullptr) {
                curr_block->next->prev = curr_block;
            }

            VALIDATE_HEADER(curr_block);
        }
        if(prev_block != nullptr && prev_block->is_free) {
            EMBER_CHECK(prev_block->next == curr_block);

            prev_block->size += curr_block->size;

            prev_block->next = curr_block->next;
            if(prev_block->next != nullptr) {
                prev_block->next->prev = prev_block;
            }

            VALIDATE_HEADER(prev_block);
        }
    }

    void global_allocator::create_new_arena(std::size_t const bytes) {
        auto *memory{ reinterpret_cast<std::byte *>(std::malloc(bytes)) };
        EMBER_THROW_IF_FAILED(memory != nullptr, exception{ "Failed to create allocate new memory for the global memory allocator." });

        arena new_arena{
            .memory = memory,
            .size   = bytes,
        };

        memory_arenas.emplace_back(std::move(new_arena));

        std::size_t const arena_index{ memory_arenas.size() - 1 };
        std::size_t constexpr block_offset{ 0 };
        create_new_block(arena_index, block_offset, bytes);
    }
}