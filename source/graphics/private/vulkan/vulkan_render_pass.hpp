#pragma once

#include "ember/graphics/render_pass.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_render_pass : public render_pass {
        //VARIABLES
    private:
        VkDevice device{ VK_NULL_HANDLE };
        VkRenderPass vk_render_pass{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_render_pass() = delete;
        inline vulkan_render_pass(VkDevice device, VkRenderPass vk_render_pass);

        vulkan_render_pass(vulkan_render_pass const &other) = delete;
        inline vulkan_render_pass(vulkan_render_pass &&other) noexcept;

        vulkan_render_pass &operator=(vulkan_render_pass const &other) = delete;
        inline vulkan_render_pass &operator=(vulkan_render_pass &&other) noexcept;

        inline ~vulkan_render_pass() override;

        inline VkRenderPass get_render_pass() const;

        static VkAttachmentLoadOp convert_load_op(load_operation const load_op);
        static VkAttachmentStoreOp convert_store_op(store_operation const store_op);
    };
}

#include "vulkan_render_pass.inl"