#include "host_memory_allocator.hpp"

namespace ember::graphics {
    device_memory_allocator::block::block(VkDevice device, VkDeviceMemory memory, VkDeviceSize bytes, std::uint32_t memory_type_index)
        : device{ device }
        , memory{ memory }
        , bytes{ bytes }
        , memory_type_index{ memory_type_index } {
        chunks.emplace_back(0, bytes, memory);
    }

    device_memory_allocator::block::block(block &&other) noexcept
        : device{ other.device }
        , memory{ other.memory }
        , bytes{ other.bytes }
        , memory_type_index{ other.memory_type_index }
        , chunks{ std::move(other.chunks) } {
        other.memory = VK_NULL_HANDLE;//Make sure the moved block no longer points to our memory
    }

    device_memory_allocator::block &device_memory_allocator::block::operator=(block &&other) noexcept {
        device            = other.device;
        memory            = other.memory;
        bytes             = other.bytes;
        memory_type_index = other.memory_type_index;
        chunks            = std::move(other.chunks);

        other.memory = VK_NULL_HANDLE;//Make sure the moved block no longer points to our memory

        return *this;
    }

    device_memory_allocator::block::~block() {
        if(memory != VK_NULL_HANDLE) {
            vkFreeMemory(device, memory, &global_host_allocation_callbacks);
        }
    }

    VkDeviceSize device_memory_allocator::block::get_bytes() const {
        return bytes;
    }

    std::uint32_t device_memory_allocator::block::get_memory_type_index() const {
        return memory_type_index;
    }

    device_memory_allocator::device_memory_allocator(VkDevice logical_device, VkPhysicalDevice physical_device)
        : logical_device{ logical_device }
        , physical_device{ physical_device } {
    }

    device_memory_allocator::device_memory_allocator(device_memory_allocator &&other) noexcept = default;

    device_memory_allocator &device_memory_allocator::operator=(device_memory_allocator &&other) noexcept = default;

    device_memory_allocator::~device_memory_allocator() = default;
}