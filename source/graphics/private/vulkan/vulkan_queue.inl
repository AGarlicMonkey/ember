#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_queue::vulkan_queue(vulkan_queue &&other) noexcept = default;

    vulkan_queue &vulkan_queue::operator=(vulkan_queue &&other) noexcept = default;
}