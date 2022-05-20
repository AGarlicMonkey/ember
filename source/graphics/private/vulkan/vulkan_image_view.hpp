#pragma once

#include "ember/graphics/image_view.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_image_view : public image_view {
        //VARIABLES
    private:
        descriptor desc{};

        VkDevice device{ VK_NULL_HANDLE };
        VkImageView handle{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_image_view() = delete;
        inline vulkan_image_view(descriptor desc, VkDevice device, VkImageView handle);

        vulkan_image_view(vulkan_image_view const &other) = delete;
        vulkan_image_view(vulkan_image_view &&other) noexcept= delete;

        vulkan_image_view &operator=(vulkan_image_view const &other) = delete;
        vulkan_image_view &operator=(vulkan_image_view &&other) noexcept= delete;

        inline ~vulkan_image_view() override;

        descriptor const &get_descriptor() const override;

        inline VkImageView get_handle() const;

        static VkImageViewType convert_type(type const type);
    };
}

#include "vulkan_image_view.inl"