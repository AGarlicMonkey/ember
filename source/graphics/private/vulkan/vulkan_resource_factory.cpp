#include "vulkan_resource_factory.hpp"

#include "ember/graphics/exception.hpp"
#include "ember/graphics/resource.hpp"
#include "host_memory_allocator.hpp"
#include "verification.hpp"
#include "vulkan_buffer.hpp"
#include "vulkan_image.hpp"
#include "vulkan_render_pass.hpp"

#include <array>
#include <ember/containers/array.hpp>

using namespace ember::memory;

namespace {
#if EMBER_GRAPHICS_DEBUG_UTILITIES
    PFN_vkSetDebugUtilsObjectNameEXT fp_vkSetDebugUtilsObjectNameEXT;
#endif

    VkMemoryPropertyFlags get_memory_property_flags(ember::graphics::memory_type memory_type) {
        switch(memory_type) {
            case ember::graphics::memory_type::host_visible:
                return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;//Including HOST_COHERENT here as this makes mapping memory more simple
            case ember::graphics::memory_type::device_local:
                return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            default:
                EMBER_CHECK(false);
                return 0;
        }
    }
}

#if EMBER_GRAPHICS_DEBUG_UTILITIES
    #define SET_RESOURCE_NAME(object_handle, object_type, object_name)                                                   \
        {                                                                                                                \
            VkDebugUtilsObjectNameInfoEXT const name_info{                                                               \
                .sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,                                      \
                .pNext        = nullptr,                                                                                 \
                .objectType   = object_type,                                                                             \
                .objectHandle = reinterpret_cast<std::uint64_t>(object_handle),                                          \
                .pObjectName  = object_name,                                                                             \
            };                                                                                                           \
            EMBER_VULKAN_VERIFY_RESULT(fp_vkSetDebugUtilsObjectNameEXT(device, &name_info), "Failed to name VkBuffer."); \
        }
#else
    #define SET_RESOURCE_NAME(object_handle, object_type, object_name)
#endif

namespace ember::graphics {
    vulkan_resource_factory::vulkan_resource_factory(VkInstance instance, VkDevice device, queue_family_indices family_indices, device_memory_allocator *memory_allocator)
        : device{ device }
        , family_indices{ std::move(family_indices) }
        , memory_allocator{ memory_allocator } {
#if EMBER_GRAPHICS_DEBUG_UTILITIES
        if(fp_vkSetDebugUtilsObjectNameEXT == nullptr) {
            fp_vkSetDebugUtilsObjectNameEXT = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
            EMBER_CHECK(fp_vkSetDebugUtilsObjectNameEXT != nullptr);
        }
#endif
    }

    unique_ptr<buffer> vulkan_resource_factory::create_buffer(buffer::descriptor const &descriptor, std::string_view name) const {
        //TODO: custom fixed size array
        std::array const shared_queue_indices{ family_indices.graphics, family_indices.compute, family_indices.transfer };
        bool const is_exclusive{ descriptor.sharing_mode == sharing_mode::exclusive };

        VkBufferCreateInfo const create_info{
            .sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext                 = nullptr,
            .flags                 = 0,
            .size                  = descriptor.bytes,
            .usage                 = vulkan_buffer::convert_usage_flags(descriptor.usage_flags),
            .sharingMode           = is_exclusive ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = is_exclusive ? 0 : static_cast<std::uint32_t>(shared_queue_indices.size()),
            .pQueueFamilyIndices   = is_exclusive ? nullptr : shared_queue_indices.data(),
        };

        VkBuffer buffer_handle{ VK_NULL_HANDLE };
        EMBER_VULKAN_VERIFY_RESULT(vkCreateBuffer(device, &create_info, &global_host_allocation_callbacks, &buffer_handle), "Failed to create VkBuffer.");

        VkMemoryRequirements memory_requirements{};
        vkGetBufferMemoryRequirements(device, buffer_handle, &memory_requirements);

        device_memory_allocator::chunk const *allocated_chunk{ memory_allocator->alloc(memory_requirements, get_memory_property_flags(descriptor.memory_type)) };
        EMBER_THROW_IF_FAILED(allocated_chunk != nullptr, resource_creation_exception{ "Failed to create buffer. Could not allocate any device memory." });
        vkBindBufferMemory(device, buffer_handle, allocated_chunk->memory, allocated_chunk->offset);

        SET_RESOURCE_NAME(buffer_handle, VK_OBJECT_TYPE_BUFFER, name.data());

        return make_unique<vulkan_buffer>(device, buffer_handle, memory_allocator, allocated_chunk);
    }

    unique_ptr<render_pass> vulkan_resource_factory::create_render_pass(render_pass::descriptor const &descriptor, std::string_view name) const {
        //Convert attachments
        std::size_t const colour_attachment_size{ descriptor.colour_attachments.size() };
        containers::array<VkAttachmentDescription> attachments(colour_attachment_size);
        for(std::size_t i{ 0 }; i < colour_attachment_size; ++i) {
            attachments[i] = VkAttachmentDescription{
                .format         = vulkan_image::convert_format(descriptor.colour_attachments[i].format),
                .samples        = VK_SAMPLE_COUNT_1_BIT,
                .loadOp         = vulkan_render_pass::convert_load_op(descriptor.colour_attachments[i].load_op),
                .storeOp        = vulkan_render_pass::convert_store_op(descriptor.colour_attachments[i].store_op),
                .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout  = vulkan_image::convert_layout(descriptor.colour_attachments[i].initial_layout),
                .finalLayout    = vulkan_image::convert_layout(descriptor.colour_attachments[i].final_layout),
            };
        }
        if(descriptor.depth_attachment.has_value()) {
            attachments.emplace_back(VkAttachmentDescription{
                .format         = vulkan_image::convert_format(descriptor.depth_attachment->format),
                .samples        = VK_SAMPLE_COUNT_1_BIT,
                .loadOp         = vulkan_render_pass::convert_load_op(descriptor.depth_attachment->load_op),
                .storeOp        = vulkan_render_pass::convert_store_op(descriptor.depth_attachment->store_op),
                .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout  = vulkan_image::convert_layout(descriptor.depth_attachment->initial_layout),
                .finalLayout    = vulkan_image::convert_layout(descriptor.depth_attachment->final_layout),
            });
        }

        containers::array<VkAttachmentReference> colour_attachment_references(colour_attachment_size);
        VkAttachmentReference depth_stencil_attachment_reference{};
        for(size_t i{ 0 }; i < colour_attachment_size; ++i) {
            colour_attachment_references[i] = VkAttachmentReference{
                .attachment = static_cast<std::uint32_t>(i),
                .layout     = vulkan_image::convert_layout(descriptor.colour_attachments[i].used_layout),
            };
        }
        if(descriptor.depth_attachment.has_value()) {
            depth_stencil_attachment_reference.attachment = static_cast<std::uint32_t>(attachments.size() - 1);
            depth_stencil_attachment_reference.layout     = vulkan_image::convert_layout(descriptor.depth_attachment->used_layout);
        }

        VkSubpassDescription const subpass{
            .flags                   = 0,
            .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount    = 0,
            .pInputAttachments       = nullptr,
            .colorAttachmentCount    = static_cast<std::uint32_t>(colour_attachment_references.size()),
            .pColorAttachments       = colour_attachment_references.data(),
            .pResolveAttachments     = nullptr,
            .pDepthStencilAttachment = descriptor.depth_attachment.has_value() ? &depth_stencil_attachment_reference : nullptr,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments    = nullptr,
        };

        VkSubpassDependency constexpr dependecy{
            .srcSubpass    = VK_SUBPASS_EXTERNAL,
            .dstSubpass    = 0,
            .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = 0,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        };

        VkRenderPassCreateInfo const render_pass_info{
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = static_cast<std::uint32_t>(attachments.size()),
            .pAttachments    = attachments.data(),
            .subpassCount    = 1,
            .pSubpasses      = &subpass,
            .dependencyCount = 1,
            .pDependencies   = &dependecy,
        };

        VkRenderPass render_pass{ nullptr };
        EMBER_VULKAN_VERIFY_RESULT(vkCreateRenderPass(device, &render_pass_info, &global_host_allocation_callbacks, &render_pass), "Failed to create RenderPass.");

        SET_RESOURCE_NAME(render_pass, VK_OBJECT_TYPE_RENDER_PASS, name.data());

        return make_unique<vulkan_render_pass>(device, render_pass);
    }
}