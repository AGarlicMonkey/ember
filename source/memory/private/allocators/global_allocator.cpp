#include "allocators/global_allocator.hpp"

#include "ember/memory/memory.hpp"

#include <ember/core/exception.hpp>
#include <ember/core/log.hpp>

EMBER_LOG_CATEGORY(EmberGlobalAllocator)

static size_t constexpr arena_size{ EMBER_GB(1) };
static ::ember::memory::global_allocator *instance{ nullptr };

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

        std::size_t const total_allocation_size{ sizeof(block) + bytes + alignment };//Allow enough room for the worst case aliognment
        std::byte *memory{ nullptr };

        for(std::size_t i{ 0 }; i < memory_arenas.size(); ++i) {
            arena &arena{ memory_arenas[i] };

            if(arena.size < bytes) {
                continue;
            }

            for(auto *const curr_block : arena.free_list) {
                EMBER_CHECK(curr_block->is_free);

                if(curr_block->size >= total_allocation_size) {
                    curr_block->is_free = false;
                    remove_block_from_free_list(arena.free_list, curr_block);

                    //If we have space left over then insert it as a new block
                    if(size_t const remaining_space{ curr_block->size - total_allocation_size }; remaining_space > 0) {
                        curr_block->size = total_allocation_size;

                        size_t const new_block_offset{ curr_block->block_offset + curr_block->size };
                        block *const new_block{ create_new_block(i, new_block_offset, remaining_space) };
                        block *const next_block{ curr_block->next };

                        curr_block->next = new_block;
                        new_block->next  = next_block;

                        if(next_block != nullptr) {
                            next_block->prev = new_block;
                        }
                        new_block->prev = curr_block;
                    }

                    //We might have already adjusted the alignment of this block so put it back
                    curr_block->data_offset = curr_block->block_offset;

                    std::size_t const remaining_alignment{ alignment != 0 ? curr_block->block_offset % alignment : 0 };
                    std::size_t const alignment_offset{ remaining_alignment != 0 ? alignment - remaining_alignment : 0 };

                    curr_block->data_offset += alignment_offset;

                    memory = arena.memory + curr_block->data_offset;
                    break;
                }
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

    void global_allocator::free(std::byte *&memory) {
        std::scoped_lock lock{ allocator_mutex };

        if(memory == nullptr) {
            return;
        }

        block *const curr_block{ get_block_from_memory(memory) };
        block *const next_block{ curr_block->next };
        block *const prev_block{ curr_block->prev };
        arena &arena{ memory_arenas[curr_block->arena_index] };

        curr_block->is_free = true;
        arena.free_list.push_back(curr_block);

        //Attempt to merge the freed memory into either the left or right blocks or both
        if(next_block != nullptr && next_block->is_free) {
            remove_block_from_free_list(arena.free_list, next_block);

            curr_block->size += next_block->size;

            curr_block->next = next_block->next;
            if(curr_block->next != nullptr) {
                curr_block->next->prev = curr_block;
            }
        }
        if(prev_block != nullptr && prev_block->is_free) {
            remove_block_from_free_list(arena.free_list, curr_block);

            prev_block->size += curr_block->size;

            prev_block->next = curr_block->next;
            if(prev_block->next != nullptr) {
                prev_block->next->prev = prev_block;
            }
        }

        memory = nullptr;
    }

    global_allocator::block *global_allocator::create_new_block(std::size_t const arena_index, std::size_t const offset, std::size_t const bytes) {
        arena &arena{ memory_arenas[arena_index] };

        block *const new_block{ reinterpret_cast<block *>(arena.memory + offset) };
        *new_block = block{
            .arena_index  = arena_index,
            .block_offset = sizeof(block) + offset,
            .data_offset  = sizeof(block) + offset,
            .size         = bytes,
        };

        EMBER_CHECK(new_block->next == nullptr);
        EMBER_CHECK(new_block->prev == nullptr);

        arena.free_list.push_back(new_block);

        return new_block;
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

    global_allocator::block *global_allocator::get_block_from_memory(std::byte const *const memory) {
        for(auto &arena : memory_arenas) {
            block *curr_block{ reinterpret_cast<block *>(arena.memory) };
            while(curr_block != nullptr) {
                if((arena.memory + curr_block->data_offset) == memory) {
                    return curr_block;
                }

                curr_block = curr_block->next;
            }
        }

        EMBER_THROW(exception{ "Unable to find memory inside current allocator" });
        return nullptr;
    }

    void global_allocator::remove_block_from_free_list(std::vector<block *> &free_list, block *const block) {
        for(std::size_t i{ 0 }; i < free_list.size(); ++i) {
            if(free_list[i] == block) {
                if(i < free_list.size() - 1) {
                    free_list[i] = free_list.back();
                }
                free_list.pop_back();

                return;
            }
        }
    }
}