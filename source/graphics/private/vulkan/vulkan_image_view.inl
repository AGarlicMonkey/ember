#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_image_view::vulkan_image_view(descriptor desc, VkDevice device, VkImageView handle)
        : desc{ desc }
        , device{ device }
        , handle{ handle } {
    }

    vulkan_image_view::~vulkan_image_view() {
        vkDestroyImageView(device, handle, &global_host_allocation_callbacks);
    }

    VkImageView vulkan_image_view::get_handle() const {
        return handle;
    }
}