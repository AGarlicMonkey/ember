#pragma once

#include "ember/graphics/descriptor_pool.hpp"

#include <vulkan/vulkan.h>

namespace ember::inline graphics {
    class vulkan_descriptor_pool : public descriptor_pool {
        //VARIABLES
    private:
        descriptor desc{};

        VkDevice device{ VK_NULL_HANDLE };
        VkDescriptorPool handle{ VK_NULL_HANDLE };

        //FUINCTIONS
    public:
        vulkan_descriptor_pool() = delete;
        inline vulkan_descriptor_pool(descriptor desc, VkDevice device, VkDescriptorPool handle);

        vulkan_descriptor_pool(vulkan_descriptor_pool const &other) = delete;
        vulkan_descriptor_pool(vulkan_descriptor_pool &&other) noexcept = delete;

        vulkan_descriptor_pool &operator=(vulkan_descriptor_pool const &other) = delete;
        vulkan_descriptor_pool &operator=(vulkan_descriptor_pool &&other) noexcept = delete;

        inline ~vulkan_descriptor_pool() override;

        descriptor const &get_descriptor() const override;

        unique_ptr<descriptor_set> allocate_descriptor_set(descriptor_set_layout const &layout) override;
        array<unique_ptr<descriptor_set>> allocate_descriptor_sets(array<descriptor_set_layout const *> const &layouts) override;

        void reset() override;

        inline VkDescriptorPool get_handle() const;

        static VkDescriptorPoolCreateFlags convert_flags(flag const flags);
    };
}

#include "vulkan_descriptor_pool.inl"