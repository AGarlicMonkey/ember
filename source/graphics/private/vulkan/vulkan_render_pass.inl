#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_render_pass::vulkan_render_pass(VkDevice device, VkRenderPass vk_render_pass)
        : device{ device }
        , vk_render_pass{ vk_render_pass } {
    }

    vulkan_render_pass::vulkan_render_pass(vulkan_render_pass &&other) noexcept = default;

    vulkan_render_pass &vulkan_render_pass::operator=(vulkan_render_pass &&other) noexcept = default;

    vulkan_render_pass::~vulkan_render_pass() {
        vkDestroyRenderPass(device, vk_render_pass, &global_host_allocation_callbacks);
    }

    VkRenderPass vulkan_render_pass::get_render_pass() const {
        return vk_render_pass;
    }
}