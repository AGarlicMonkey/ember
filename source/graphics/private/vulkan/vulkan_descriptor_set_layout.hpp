#pragma once

#include "ember/graphics/descriptor_set_layout.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_descriptor_set_layout : public descriptor_set_layout {
        //VARIABLES
    private:
        VkDevice device{ VK_NULL_HANDLE };
        VkDescriptorSetLayout layout{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_descriptor_set_layout() = delete;
        inline vulkan_descriptor_set_layout(VkDevice device, VkDescriptorSetLayout layout);

        vulkan_descriptor_set_layout(vulkan_descriptor_set_layout const &other) = delete;
        inline vulkan_descriptor_set_layout(vulkan_descriptor_set_layout &&other) noexcept;

        vulkan_descriptor_set_layout &operator=(vulkan_descriptor_set_layout const &other) = delete;
        inline vulkan_descriptor_set_layout &operator=(vulkan_descriptor_set_layout &&other) noexcept;

        inline ~vulkan_descriptor_set_layout() override;

        inline VkDescriptorSetLayout get_layout() const;

        static VkDescriptorType convert_descriptor_type(descriptor_type const type);
    };
}

#include "vulkan_descriptor_set_layout.inl"