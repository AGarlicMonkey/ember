#include "host_memory_allocator.hpp"

namespace ember::inline graphics {
    vulkan_descriptor_set_layout::vulkan_descriptor_set_layout(descriptor desc, VkDevice device, VkDescriptorSetLayout handle)
        : desc{ desc }
        , device{ device }
        , handle{ handle } {
    }

    vulkan_descriptor_set_layout::~vulkan_descriptor_set_layout() {
        vkDestroyDescriptorSetLayout(device, handle, &global_host_allocation_callbacks);
    }

    VkDescriptorSetLayout vulkan_descriptor_set_layout::get_handle() const {
        return handle;
    }
}