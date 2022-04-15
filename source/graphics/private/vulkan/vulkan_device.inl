namespace ember::graphics {
    vulkan_device::vulkan_device(vulkan_device &&other) noexcept = default;

    vulkan_device &vulkan_device::operator=(vulkan_device &&other) noexcept = default;
}