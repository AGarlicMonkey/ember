#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_swapchain::~vulkan_swapchain() {
        vkDestroySwapchainKHR(device, handle, &global_host_allocation_callbacks);
        vkDestroySurfaceKHR(instance, surface, &global_host_allocation_callbacks);
    }

    VkSwapchainKHR vulkan_swapchain::get_handle() const {
        return handle;
    }
}