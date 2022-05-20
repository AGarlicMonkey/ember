#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_fence::vulkan_fence(descriptor desc, VkDevice device, VkFence handle)
        : desc{ desc }
        , device{ device }
        , handle{ handle } {
    }

    vulkan_fence::~vulkan_fence() {
        vkDestroyFence(device, handle, &global_host_allocation_callbacks);
    }

    VkFence vulkan_fence::get_handle() const {
        return handle;
    }
}