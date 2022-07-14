namespace ember::inline graphics {
    vulkan_shader_cache::vulkan_shader_cache(VkDevice device)
        : device{ device } {
    }

    vulkan_shader_cache::vulkan_shader_cache(vulkan_shader_cache &&other) noexcept = default;

    vulkan_shader_cache &vulkan_shader_cache::operator=(vulkan_shader_cache &&other) noexcept = default;

    vulkan_shader_cache::~vulkan_shader_cache() = default;
}