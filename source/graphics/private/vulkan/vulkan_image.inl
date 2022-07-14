#include "host_memory_allocator.hpp"

namespace ember::inline graphics {
    vulkan_image::vulkan_image(descriptor desc, VkDevice device, VkImage handle, device_memory_allocator *memory_allocator, device_memory_allocator::chunk const *allocated_chunk)
        : desc{ desc }
        , device{ device }
        , handle{ handle }
        , memory_allocator{ memory_allocator }
        , allocated_chunk{ allocated_chunk } {
    }

    vulkan_image::vulkan_image(descriptor desc, VkDevice device, VkImage handle)
        : desc{ desc }
        , device{ device }
        , handle{ handle } {
    }

    vulkan_image::~vulkan_image() {
        if(memory_allocator != nullptr) {
            vkDestroyImage(device, handle, &global_host_allocation_callbacks);
            memory_allocator->free(allocated_chunk);
        }
    }

    VkImage vulkan_image::get_handle() const {
        return handle;
    }
}