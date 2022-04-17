#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_swapchain::vulkan_swapchain(vulkan_swapchain &&other) noexcept = default;

    vulkan_swapchain &vulkan_swapchain::operator=(vulkan_swapchain &&other) noexcept = default;

    vulkan_swapchain::~vulkan_swapchain() {
        vkDestroySwapchainKHR(device, swapchain, &global_host_allocation_callbacks);
        vkDestroySurfaceKHR(instance, surface, &global_host_allocation_callbacks);
    }

    VkSwapchainKHR vulkan_swapchain::get_swapchain() const {
        return swapchain;
    }
}