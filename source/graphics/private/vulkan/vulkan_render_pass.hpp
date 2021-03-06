#pragma once

#include "ember/graphics/render_pass.hpp"

#include <vulkan/vulkan.h>

namespace ember::inline graphics {
    class vulkan_render_pass : public render_pass {
        //VARIABLES
    private:
        descriptor desc{};

        VkDevice device{ VK_NULL_HANDLE };
        VkRenderPass handle{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_render_pass() = delete;
        inline vulkan_render_pass(descriptor desc, VkDevice device, VkRenderPass handle);

        vulkan_render_pass(vulkan_render_pass const &other) = delete;
        vulkan_render_pass(vulkan_render_pass &&other) noexcept = delete;

        vulkan_render_pass &operator=(vulkan_render_pass const &other) = delete;
        vulkan_render_pass &operator=(vulkan_render_pass &&other) noexcept = delete;

        inline ~vulkan_render_pass() override;

        descriptor const &get_descriptor() const override;

        inline VkRenderPass get_handle() const;

        static VkAttachmentLoadOp convert_load_op(load_operation const load_op);
        static VkAttachmentStoreOp convert_store_op(store_operation const store_op);
    };
}

#include "vulkan_render_pass.inl"