#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_descriptor_set_layout::vulkan_descriptor_set_layout(VkDevice device, VkDescriptorSetLayout layout)
        : device{ device }
        , layout{ layout } {
    }

    vulkan_descriptor_set_layout::vulkan_descriptor_set_layout(vulkan_descriptor_set_layout &&other) noexcept = default;

    vulkan_descriptor_set_layout &vulkan_descriptor_set_layout::operator=(vulkan_descriptor_set_layout &&other) noexcept = default;

    vulkan_descriptor_set_layout::~vulkan_descriptor_set_layout() {
        vkDestroyDescriptorSetLayout(device, layout, &global_host_allocation_callbacks);
    }

    VkDescriptorSetLayout vulkan_descriptor_set_layout::get_layout() const {
        return layout;
    }
}