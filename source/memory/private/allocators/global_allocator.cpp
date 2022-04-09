#include "allocators/global_allocator.hpp"

#include "ember/memory/memory.hpp"

#include <ember/core/exception.hpp>
#include <ember/core/log.hpp>

EMBER_LOG_CATEGORY(EmberGlobalAllocator)

static size_t constexpr initial_size{ EMBER_MB(1) };
static ::ember::memory::global_allocator *instance{ nullptr };

namespace ember::memory {
    global_allocator::global_allocator(std::size_t initial_memory_size)
        : size{ initial_memory_size } {
        backing_memory = reinterpret_cast<std::byte *>(malloc(initial_memory_size));
        EMBER_THROW_IF_FAILED(backing_memory != nullptr, exception{ "Failed to create backing memory for global allocator." });

        free_list.emplace_back(block{ .size = initial_memory_size });

        EMBER_LOG(EmberGlobalAllocator, log_level::info, "Initialised global memory pool with {0} bytes.", initial_memory_size);
    }

    global_allocator &global_allocator::get() {
        if(instance == nullptr) {
            instance = new global_allocator{ initial_size };
        }

        return *instance;
    }

    std::byte *global_allocator::alloc(std::size_t bytes, std::size_t alignment) {
        std::size_t const total_allocation_size{ bytes + alignment };//Allow enough room for the worst case aliognment
        std::byte *memory{ nullptr };

        for(auto current_block{ free_list.begin() }; current_block != free_list.end(); ++current_block) {
            if(current_block->is_free && current_block->size >= total_allocation_size) {
                current_block->is_free = false;

                //If we have space left over then insert it as a new block
                if(size_t const remaining_space{ current_block->size - total_allocation_size }; remaining_space > 0) {
                    current_block->size = total_allocation_size;

                    size_t const new_block_offset{ current_block->offset + current_block->size };
                    free_list.insert(std::next(current_block), block{
                                                                   .offset         = new_block_offset,
                                                                   .aligned_offset = new_block_offset,
                                                                   .size           = remaining_space,
                                                               });
                }

                std::size_t const remaining_alignment{ alignment != 0 ? current_block->offset % alignment : 0 };
                std::size_t const alignment_offset{ remaining_alignment != 0 ? alignment - remaining_alignment : 0 };

                current_block->aligned_offset = current_block->offset + alignment_offset;

                memory = backing_memory + current_block->aligned_offset;
                break;
            }
        }

        if(memory == nullptr) {
            //TODO: resize
        }

        return memory;
    }

    void global_allocator::free(std::byte *&memory) {
        if(memory == nullptr) {
            return;
        }

        for(auto block{ free_list.begin() }; block != free_list.end(); ++block) {
            if(backing_memory + block->aligned_offset == memory) {

                bool remove_index{ false };
                bool remove_right{ false };

                //Attempt to merge the new memory into either the left or right blocks or both
                if(auto right_block{ std::next(block) }; right_block != free_list.end() && right_block->is_free) {
                    block->size += right_block->size;
                    remove_right = true;
                }
                if(block != free_list.begin()) {
                    if(auto left_block{ std::prev(block) }; left_block->is_free) {
                        left_block->size += block->size;
                        remove_index = true;
                    }
                }

                if(remove_right) {
                    free_list.erase(std::next(block));
                }
                if(remove_index) {
                    free_list.erase(block);
                }

                block->is_free = true;
                memory = nullptr;
                break;
            }
        }
    }
}