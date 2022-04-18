#include "vulkan_render_pass.hpp"

#include "ember/core/log.hpp"

namespace ember::graphics {
    VkAttachmentLoadOp vulkan_render_pass::convert_load_op(load_operation const load_op) {
        switch(load_op) {
            case load_operation::load:
                return VK_ATTACHMENT_LOAD_OP_LOAD;
            case load_operation::clear:
                return VK_ATTACHMENT_LOAD_OP_CLEAR;
            case load_operation::dont_care:
                return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            default:
                EMBER_CHECK(false);
                return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        }
    }

    VkAttachmentStoreOp vulkan_render_pass::convert_store_op(store_operation const store_op) {
        switch(store_op) {
            case store_operation::store:
                return VK_ATTACHMENT_STORE_OP_STORE;
            case store_operation::dont_care:
                return VK_ATTACHMENT_STORE_OP_DONT_CARE;
            default:
                EMBER_CHECK(false);
                return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        }
    }
}