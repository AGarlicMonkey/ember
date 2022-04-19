#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_image_view::vulkan_image_view(VkDevice device, VkImageView image_view_handle)
        : device{ device }
        , image_view_handle{ image_view_handle } {
    }

    vulkan_image_view::vulkan_image_view(vulkan_image_view &&other) noexcept = default;

    vulkan_image_view &vulkan_image_view ::operator=(vulkan_image_view &&other) noexcept = default;

    vulkan_image_view::~vulkan_image_view() {
        vkDestroyImageView(device, image_view_handle, &global_host_allocation_callbacks);
    }

    VkImageView vulkan_image_view::get_image_view() const {
        return image_view_handle;
    }
}