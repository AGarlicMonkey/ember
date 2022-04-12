namespace ember::graphics {
    vulkan_instance::vulkan_instance(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, VkAllocationCallbacks global_allocator)
        : instance{ instance }
        , debug_messenger{ debug_messenger }
        , global_allocator{ global_allocator } {
    }

    vulkan_instance::vulkan_instance(vulkan_instance &&other) noexcept = default;

    vulkan_instance &vulkan_instance::operator=(vulkan_instance &&other) noexcept = default;
}