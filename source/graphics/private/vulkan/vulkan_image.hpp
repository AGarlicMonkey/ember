#pragma once

#include "device_memory_allocator.hpp"
#include "ember/graphics/image.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_image : public image {
        //VARIABLES
    private:
        descriptor desc{};

        VkDevice device{ VK_NULL_HANDLE };
        VkImage handle{ VK_NULL_HANDLE };

        device_memory_allocator *memory_allocator{ nullptr };
        device_memory_allocator::chunk const *allocated_chunk{ nullptr };

        //FUNCTIONS
    public:
        vulkan_image() = delete;
        inline vulkan_image(descriptor desc, VkDevice device, VkImage handle, device_memory_allocator *memory_allocator, device_memory_allocator::chunk const *allocated_chunk);
        inline vulkan_image(descriptor desc, VkDevice device, VkImage handle);

        vulkan_image(vulkan_image const &other)     = delete;
        vulkan_image(vulkan_image &&other) noexcept = delete;

        vulkan_image &operator=(vulkan_image const &other) = delete;
        vulkan_image &operator=(vulkan_image &&other) noexcept = delete;

        inline ~vulkan_image() override;

        descriptor const &get_descriptor() const override;

        inline VkImage get_handle() const;

        static VkImageUsageFlags convert_usage(usage_mode const usage);
        static VkImageType convert_type(type const type);
        static format convert_format(VkFormat const format);
        static VkFormat convert_format(format const format);
        static VkImageLayout convert_layout(layout const layout);
    };
}

#include "vulkan_image.inl"