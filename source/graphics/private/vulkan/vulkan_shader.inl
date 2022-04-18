#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_shader::vulkan_shader(VkDevice device, VkShaderModule module)
        : device{ device }
        , module{ module } {
    }

    vulkan_shader::vulkan_shader(vulkan_shader &&other) noexcept = default;

    vulkan_shader &vulkan_shader::operator=(vulkan_shader &&other) noexcept = default;

    vulkan_shader::~vulkan_shader() {
        vkDestroyShaderModule(device, module, &global_host_allocation_callbacks);
    }

    inline VkShaderModule vulkan_shader::get_module() const {
        return module;
    }
}