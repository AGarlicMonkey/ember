namespace ember::graphics {
    vulkan_descriptor_set::vulkan_descriptor_set(VkDevice device, VkDescriptorSet handle)
        : device{ device }
        , handle{ handle } {
    }

    vulkan_descriptor_set::~vulkan_descriptor_set() = default;

    VkDescriptorSet vulkan_descriptor_set::get_handle() const {
        return handle;
    }
}