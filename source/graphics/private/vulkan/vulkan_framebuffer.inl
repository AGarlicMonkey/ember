#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_framebuffer::vulkan_framebuffer(descriptor desc, VkDevice device, VkFramebuffer handle)
        : desc{ desc }
        , device{ device }
        , handle{ handle } {
    }

    vulkan_framebuffer::vulkan_framebuffer(vulkan_framebuffer &&other) noexcept = default;

    vulkan_framebuffer &vulkan_framebuffer::operator=(vulkan_framebuffer &&other) noexcept = default;

    vulkan_framebuffer::~vulkan_framebuffer() {
        vkDestroyFramebuffer(device, handle, &global_host_allocation_callbacks);
    }

    inline VkFramebuffer vulkan_framebuffer::get_handle() const {
        return handle;
    }
}