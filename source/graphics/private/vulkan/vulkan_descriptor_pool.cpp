#include "vulkan_descriptor_pool.hpp"

#include "resource_cast.hpp"
#include "verification.hpp"
#include "vulkan_descriptor_set.hpp"
#include "vulkan_descriptor_set_layout.hpp"

namespace ember::inline graphics {
    descriptor_pool::descriptor const &vulkan_descriptor_pool::get_descriptor() const {
        return desc;
    }

    unique_ptr<descriptor_set> vulkan_descriptor_pool::allocate_descriptor_set(descriptor_set_layout const &layout) {
        return std::move(allocate_descriptor_sets({ &layout })[0]);
    }

    array<unique_ptr<descriptor_set>> vulkan_descriptor_pool::allocate_descriptor_sets(array<descriptor_set_layout const *> const &layouts) {
        std::size_t const num_sets{ layouts.size() };

        array<VkDescriptorSetLayout> layout_handles(num_sets);
        for(std::size_t i{ 0 }; i < num_sets; ++i) {
            layout_handles[i] = resource_cast<vulkan_descriptor_set_layout const>(layouts[i])->get_handle();
        }

        VkDescriptorSetAllocateInfo const allocation_info{
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext              = nullptr,
            .descriptorPool     = handle,
            .descriptorSetCount = static_cast<std::uint32_t>(num_sets),
            .pSetLayouts        = layout_handles.data(),
        };

        array<VkDescriptorSet> descriptor_set_handles(num_sets);
        EMBER_VULKAN_VERIFY_RESULT(vkAllocateDescriptorSets(device, &allocation_info, std::data(descriptor_set_handles)), "Could not allocate VkDescriptorSets.");

        array<unique_ptr<descriptor_set>> descriptor_sets(num_sets);
        for(std::size_t i{ 0 }; i < num_sets; ++i) {
            descriptor_sets[i] = make_unique<vulkan_descriptor_set>(device, descriptor_set_handles[i]);
        }

        return descriptor_sets;
    }

    void vulkan_descriptor_pool::reset() {
        vkResetDescriptorPool(device, handle, 0);
    }
}