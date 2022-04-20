#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_buffer::vulkan_buffer(descriptor desc, VkDevice device, VkBuffer handle, device_memory_allocator *memory_allocator, device_memory_allocator::chunk const *allocated_chunk)
        : desc{ desc }
        , device{ device }
        , handle{ handle }
        , memory_allocator{ memory_allocator }
        , allocated_chunk{ allocated_chunk } {
    }

    vulkan_buffer::vulkan_buffer(vulkan_buffer &&other) noexcept = default;

    vulkan_buffer &vulkan_buffer::operator=(vulkan_buffer &&other) noexcept = default;

    vulkan_buffer::~vulkan_buffer() {
        vkDestroyBuffer(device, handle, &global_host_allocation_callbacks);
        memory_allocator->free(allocated_chunk);
    }

    VkBuffer vulkan_buffer::get_handle() const {
        return handle;
    }
}