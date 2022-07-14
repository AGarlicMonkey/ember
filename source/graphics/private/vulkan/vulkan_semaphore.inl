#include "host_memory_allocator.hpp"

namespace ember::inline graphics {
    vulkan_semaphore::vulkan_semaphore(VkDevice device, VkSemaphore handle)
        : device{ device }
        , handle{ handle } {
    }

    vulkan_semaphore::~vulkan_semaphore() {
        vkDestroySemaphore(device, handle, &global_host_allocation_callbacks);
    }

    VkSemaphore vulkan_semaphore::get_handle() const {
        return handle;
    }
}