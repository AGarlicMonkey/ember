#pragma once

#include "ember/graphics/descriptor_set_layout.hpp"

#include <vulkan/vulkan.h>

namespace ember::inline graphics {
    class vulkan_descriptor_set_layout : public descriptor_set_layout {
        //VARIABLES
    private:
        descriptor desc{};

        VkDevice device{ VK_NULL_HANDLE };
        VkDescriptorSetLayout handle{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_descriptor_set_layout() = delete;
        inline vulkan_descriptor_set_layout(descriptor desc, VkDevice device, VkDescriptorSetLayout handle);

        vulkan_descriptor_set_layout(vulkan_descriptor_set_layout const &other)     = delete;
        vulkan_descriptor_set_layout(vulkan_descriptor_set_layout &&other) noexcept = delete;

        vulkan_descriptor_set_layout &operator=(vulkan_descriptor_set_layout const &other) = delete;
        vulkan_descriptor_set_layout &operator=(vulkan_descriptor_set_layout &&other) noexcept = delete;

        inline ~vulkan_descriptor_set_layout() override;

        descriptor const &get_descriptor() const override;

        inline VkDescriptorSetLayout get_handle() const;
    };
}

#include "vulkan_descriptor_set_layout.inl"