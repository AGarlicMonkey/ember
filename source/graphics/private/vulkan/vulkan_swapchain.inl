namespace ember::graphics {
    vulkan_swapchain::vulkan_swapchain(vulkan_swapchain &&other) noexcept = default;

    vulkan_swapchain &vulkan_swapchain::operator=(vulkan_swapchain &&other) noexcept = default;

    vulkan_swapchain::~vulkan_swapchain() {
        vkDestroySwapchainKHR(device, swapchain, &global_allocator);
        vkDestroySurfaceKHR(instance, surface, &global_allocator);
    }

    VkSwapchainKHR vulkan_swapchain::get_swapchain() const {
        return swapchain;
    }
}