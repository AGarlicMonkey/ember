#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_descriptor_pool::vulkan_descriptor_pool(descriptor desc, VkDevice device, VkDescriptorPool handle)
        : desc{ desc }
        , device{ device }
        , handle{ handle } {
    }

    vulkan_descriptor_pool::~vulkan_descriptor_pool() {
        vkDestroyDescriptorPool(device, handle, &global_host_allocation_callbacks);
    }

    VkDescriptorPool vulkan_descriptor_pool::get_handle() const {
        return handle;
    }
}