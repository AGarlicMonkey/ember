namespace ember::graphics {
    vulkan_instance::vulkan_instance(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger)
        : instance{ instance }
        , debug_messenger{ debug_messenger } {
    }

    vulkan_instance::vulkan_instance(vulkan_instance &&other) noexcept = default;

    vulkan_instance &vulkan_instance::operator=(vulkan_instance &&other) noexcept = default;
}