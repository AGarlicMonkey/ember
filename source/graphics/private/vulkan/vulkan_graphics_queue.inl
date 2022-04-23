#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_graphics_queue::vulkan_graphics_queue(vulkan_graphics_queue &&other) noexcept = default;

    vulkan_graphics_queue &vulkan_graphics_queue::operator=(vulkan_graphics_queue &&other) noexcept = default;

    vulkan_graphics_queue::~vulkan_graphics_queue() {
        vkDestroyCommandPool(logical_device, command_pool, &global_host_allocation_callbacks);
    }
}