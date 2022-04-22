#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_semaphore::vulkan_semaphore(VkDevice device, VkSemaphore handle)
        : device{ device }
        , handle{ handle } {
    }

    vulkan_semaphore::vulkan_semaphore(vulkan_semaphore &&other) noexcept = default;

    vulkan_semaphore &vulkan_semaphore::operator=(vulkan_semaphore &&other) noexcept = default;

    vulkan_semaphore::~vulkan_semaphore() {
        vkDestroySemaphore(device, handle, &global_host_allocation_callbacks);
    }

    VkSemaphore vulkan_semaphore::get_handle() const {
        return handle;
    }
}