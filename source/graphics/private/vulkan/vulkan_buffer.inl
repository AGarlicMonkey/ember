#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_buffer::vulkan_buffer(VkDevice device, VkBuffer vk_buffer, device_memory_allocator *memory_allocator, device_memory_allocator::chunk const *allocated_chunk)
        : device{ device }
        , vk_buffer{ vk_buffer }
        , memory_allocator{ memory_allocator }
        , allocated_chunk{ allocated_chunk } {
    }

    vulkan_buffer::vulkan_buffer(vulkan_buffer &&other) noexcept = default;

    vulkan_buffer &vulkan_buffer::operator=(vulkan_buffer &&other) noexcept = default;

    vulkan_buffer::~vulkan_buffer() {
        vkDestroyBuffer(device, vk_buffer, &global_host_allocation_callbacks);
        memory_allocator->free(allocated_chunk);
    }

    VkBuffer vulkan_buffer::get_buffer() const {
        return vk_buffer;
    }
}