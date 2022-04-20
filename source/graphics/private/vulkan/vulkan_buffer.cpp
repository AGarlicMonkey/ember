#include "vulkan_buffer.hpp"

namespace ember::graphics {
    buffer::descriptor const &vulkan_buffer::get_descriptor() const {
        return desc;
    }

    void *vulkan_buffer::map(std::size_t const offset, std::size_t const bytes) {
        void *host_visible_memory{ nullptr };
        vkMapMemory(device, allocated_chunk->memory, allocated_chunk->offset + offset, bytes, 0, &host_visible_memory);
        return host_visible_memory;
    }

    void vulkan_buffer::unmap() {
        vkUnmapMemory(device, allocated_chunk->memory);
    }

    VkBufferUsageFlags vulkan_buffer::convert_usage_flags(usage_mode const usage_mode) {
        using enum usage_mode;

        VkBufferUsageFlags flags{ 0 };
        if((usage_mode & transfer_source) != 0) {
            flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        }
        if((usage_mode & transfer_destination) != 0) {
            flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        }
        if((usage_mode & vertex_buffer) != 0) {
            flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        }
        if((usage_mode & index_buffer) != 0) {
            flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        }
        if((usage_mode & uniform_buffer) != 0) {
            flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        }
        if((usage_mode & storage_buffer) != 0) {
            flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        }
        return flags;
    }
}