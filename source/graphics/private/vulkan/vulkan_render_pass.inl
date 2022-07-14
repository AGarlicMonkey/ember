#include "host_memory_allocator.hpp"

namespace ember::inline graphics {
    vulkan_render_pass::vulkan_render_pass(descriptor desc, VkDevice device, VkRenderPass handle)
        : desc{ desc }
        , device{ device }
        , handle{ handle } {
    }

    vulkan_render_pass::~vulkan_render_pass() {
        vkDestroyRenderPass(device, handle, &global_host_allocation_callbacks);
    }

    VkRenderPass vulkan_render_pass::get_handle() const {
        return handle;
    }
}