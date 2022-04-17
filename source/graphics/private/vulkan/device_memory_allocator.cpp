#include "device_memory_allocator.hpp"

#include "host_memory_allocator.hpp"
#include "log.hpp"
#include "verification.hpp"

namespace {
    std::uint32_t get_memory_type_index(std::uint32_t const type_bits, VkMemoryPropertyFlags properties, VkPhysicalDevice physical_device) {
        VkPhysicalDeviceMemoryProperties memoryType{};
        vkGetPhysicalDeviceMemoryProperties(physical_device, &memoryType);

        for(uint32_t i = 0; i < memoryType.memoryTypeCount; ++i) {
            if((type_bits & (1 << i)) != 0 && (memoryType.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        EMBER_CHECK_MSG(false, "{0}: Failed to find the specified index", EMBER_FUNCTION_NAME);
        return -1;
    }

}
namespace ember::graphics {
    bool operator==(device_memory_allocator::chunk const &lhs, device_memory_allocator::chunk const &rhs) {
        return lhs.offset == rhs.offset &&
            lhs.bytes == rhs.bytes &&
            lhs.memory == rhs.memory &&
            lhs.free == rhs.free;
    }

    device_memory_allocator::chunk const *device_memory_allocator::block::alloc(VkDeviceSize const bytes, VkDeviceSize const alignment) {
        for(auto chunk_iter{ std::begin(chunks) }; chunk_iter != std::end(chunks); ++chunk_iter) {
            if(chunk_iter->free) {
                VkDeviceSize const remaining_alignment{ chunk_iter->offset % alignment };                                 //How much we're off from the alignment
                VkDeviceSize const alignment_correction{ remaining_alignment != 0 ? alignment - remaining_alignment : 0 };//How much we compensate for to achieve a multiple of alignment

                if(VkDeviceSize const chunk_bytes{ chunk_iter->bytes - alignment_correction }; chunk_bytes >= bytes) {
                    auto const prev_offset{ chunk_iter->offset };

                    chunk_iter->offset += alignment_correction;
                    chunk_iter->bytes = chunk_bytes;
                    chunk_iter->free  = false;

                    //Add back the extra bits to the chunk to the left
                    if(auto const extra_bits{ chunk_iter->offset - prev_offset }; extra_bits > 0) {
                        std::prev(chunk_iter)->bytes += extra_bits;
                    }

                    //If we have room left in the chunk, split it and put the excess back in the list.
                    if(VkDeviceSize const remaining_bytes{ chunk_iter->bytes - bytes }; remaining_bytes > 0) {
                        chunks.insert(std::next(chunk_iter), chunk{ chunk_iter->offset + bytes, remaining_bytes, memory });
                        chunk_iter->bytes = bytes;
                    }

                    return &*chunk_iter;
                }
            }
        }

        return nullptr;
    }

    bool device_memory_allocator::block::free(chunk const *&chunk_ptr) {
        for(auto curr_chunk{ std::begin(chunks) }; curr_chunk != std::end(chunks); ++curr_chunk) {
            if(*curr_chunk == *chunk_ptr) {
                curr_chunk->free = true;

                bool remove_index{ false };
                bool remove_right{ false };

                //Merge neighbouring chunks
                if(auto right_chunk{ std::next(curr_chunk) }; right_chunk != std::end(chunks) && right_chunk->free) {
                    //Merge right into us
                    curr_chunk->bytes += right_chunk->bytes;

                    remove_right = true;
                }
                if(curr_chunk != std::begin(chunks)) {
                    if(auto left_chunk{ std::prev(curr_chunk) }; left_chunk->free) {
                        //Merge ourselves into left
                        left_chunk->bytes += curr_chunk->bytes;

                        remove_index = true;
                    }
                }

                //Remove the right most first to preserve indices
                if(remove_right) {
                    chunks.erase(std::next(curr_chunk));
                }
                if(remove_index) {
                    chunks.erase(curr_chunk);
                }

                chunk_ptr = nullptr;
                return true;
            }
        }

        return false;
    }

    device_memory_allocator::chunk const *device_memory_allocator::alloc(VkMemoryRequirements const &memory_requirements, VkMemoryPropertyFlags const properties) {
        std::uint32_t const memory_type_index{ get_memory_type_index(memory_requirements.memoryTypeBits, properties, physical_device) };

        chunk const *free_chunk{ nullptr };
        for(auto &block : memory_blocks) {
            if(block.get_memory_type_index() == memory_type_index) {
                free_chunk = block.alloc(memory_requirements.size, memory_requirements.alignment);

                if(free_chunk != nullptr) {
                    break;
                }
            }
        }

        if(free_chunk == nullptr) {
            //Make sure if allocate a new block that's big enough
            VkDeviceSize const size{ std::max(memory_requirements.size, default_block_size) };

            VkMemoryAllocateInfo const info{
                .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                .pNext           = nullptr,
                .allocationSize  = size,
                .memoryTypeIndex = memory_type_index,
            };

            VkDeviceMemory memory{ VK_NULL_HANDLE };
            EMBER_VULKAN_VERIFY_RESULT(vkAllocateMemory(logical_device, &info, &global_host_allocation_callbacks, &memory), "Could not allocate memory.");

#if EMBER_GRAPHICS_TRACK_MEMORY
            {
                VkPhysicalDeviceMemoryBudgetPropertiesEXT  memory_budget_properties{
                    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT,
                };
                VkPhysicalDeviceMemoryProperties2 properties{
                    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2,
                    .pNext = &memory_budget_properties,
                };
                vkGetPhysicalDeviceMemoryProperties2(physical_device, &properties);

                std::uint32_t const heap_index{ properties.memoryProperties.memoryTypes[memory_type_index].heapIndex };
                EMBER_LOG(EmberGraphicsVulkan, log_level::trace, "Allocated block of {0} bytes from heap {1}. Current heap usage: {2} / {3}", size, heap_index, memory_budget_properties.heapUsage[heap_index], memory_budget_properties.heapBudget[heap_index]);
            }
#endif

            memory_blocks.emplace_back(logical_device, memory, size, memory_type_index);
            free_chunk = memory_blocks.back().alloc(memory_requirements.size, memory_requirements.alignment);

            EMBER_CHECK_MSG(free_chunk != nullptr, "{0}: Newly allocated Block does not have enough room", EMBER_FUNCTION_NAME);
        }

        return free_chunk;
    }

    void device_memory_allocator::free(chunk const *&chunk) {
        for(auto &block : memory_blocks) {
            if(block.free(chunk)) {
                break;
            }
        }
    }
}