#pragma once

#include "device_memory_allocator.hpp"
#include "ember/graphics/buffer.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_buffer : public buffer {
        //VARIABLES
    private:
        descriptor desc{};

        VkDevice device{ VK_NULL_HANDLE };
        VkBuffer handle{ VK_NULL_HANDLE };

        device_memory_allocator *memory_allocator{ nullptr };
        device_memory_allocator::chunk const *allocated_chunk{ nullptr };

        //FUNCTIONS
    public:
        vulkan_buffer() = delete;
        inline vulkan_buffer(descriptor desc, VkDevice device, VkBuffer handle, device_memory_allocator *memory_allocator, device_memory_allocator::chunk const *allocated_chunk);

        vulkan_buffer(vulkan_buffer const &other)     = delete;
        vulkan_buffer(vulkan_buffer &&other) noexcept = delete;

        vulkan_buffer &operator=(vulkan_buffer const &other) = delete;
        vulkan_buffer &operator=(vulkan_buffer &&other) noexcept = delete;

        inline ~vulkan_buffer() override;

        descriptor const &get_descriptor() const override;

        void *map(std::size_t const offset, std::size_t const bytes) override;

        void unmap() override;

        inline VkBuffer get_handle() const;

        static VkBufferUsageFlags convert_usage_flags(usage_mode const usage_mode);
    };
}

#include "vulkan_buffer.inl"