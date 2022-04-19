#pragma once

#include "ember/graphics/image_view.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_image_view : public image_view {
        //VARIABLES
    private:
        VkDevice device{ VK_NULL_HANDLE };
        VkImageView image_view_handle{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_image_view() = delete;
        inline vulkan_image_view(VkDevice device, VkImageView image_view_handle);

        vulkan_image_view(vulkan_image_view const &other) = delete;
        inline vulkan_image_view(vulkan_image_view &&other) noexcept;

        vulkan_image_view &operator=(vulkan_image_view const &other) = delete;
        inline vulkan_image_view &operator=(vulkan_image_view &&other) noexcept;

        inline ~vulkan_image_view() override;

        inline VkImageView get_image_view() const;

        static VkImageViewType convert_type(type const type);
    };
}

#include "vulkan_image_view.inl"