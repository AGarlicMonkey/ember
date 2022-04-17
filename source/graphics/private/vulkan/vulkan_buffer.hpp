#pragma once

#include "device_memory_allocator.hpp"
#include "ember/graphics/buffer.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_buffer : public buffer {
        //VARIABLES
    private:
        VkDevice device{ VK_NULL_HANDLE };
        VkBuffer vk_buffer{ VK_NULL_HANDLE };

        device_memory_allocator *memory_allocator{ nullptr };
        device_memory_allocator::chunk const *allocated_chunk{ nullptr };

        //FUNCTIONS
    public:
        vulkan_buffer() = delete;
        inline vulkan_buffer(VkDevice device, VkBuffer vk_buffer, device_memory_allocator *memory_allocator, device_memory_allocator::chunk const *allocated_chunk);

        vulkan_buffer(vulkan_buffer const &other) = delete;
        inline vulkan_buffer(vulkan_buffer &&other) noexcept;

        vulkan_buffer &operator=(vulkan_buffer const &other) = delete;
        inline vulkan_buffer &operator=(vulkan_buffer &&other) noexcept;

        inline ~vulkan_buffer() override;

        void *map(std::size_t const offset, std::size_t const bytes) override;

        void unmap() override;

        inline VkBuffer get_buffer() const;

        static VkBufferUsageFlags convert_usage_flags(usage_mode const usage_mode);
    };
}

#include "vulkan_buffer.inl"