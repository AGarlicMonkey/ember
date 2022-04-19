#include "vulkan_resource_factory.hpp"

#include "ember/graphics/exception.hpp"
#include "ember/graphics/resource.hpp"
#include "host_memory_allocator.hpp"
#include "resource_cast.hpp"
#include "verification.hpp"
#include "vulkan_buffer.hpp"
#include "vulkan_descriptor_set_layout.hpp"
#include "vulkan_graphics_pipeline_object.hpp"
#include "vulkan_image.hpp"
#include "vulkan_image_view.hpp"
#include "vulkan_render_pass.hpp"
#include "vulkan_shader.hpp"

#include <array>
#include <ember/containers/array.hpp>

using namespace ember::containers;
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
        , family_indices{ family_indices }
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
        EMBER_THROW_IF_FAILED(allocated_chunk != nullptr, resource_creation_exception{ "Failed to create VkBuffer. Could not allocate any device memory." });
        vkBindBufferMemory(device, buffer_handle, allocated_chunk->memory, allocated_chunk->offset);

        SET_RESOURCE_NAME(buffer_handle, VK_OBJECT_TYPE_BUFFER, name.data());

        return make_unique<vulkan_buffer>(device, buffer_handle, memory_allocator, allocated_chunk);
    }

    unique_ptr<image> vulkan_resource_factory::create_image(image::descriptor descriptor, std::string_view name) const {
        //TODO: custom static array
        std::array const shared_queue_indices{ family_indices.graphics, family_indices.compute, family_indices.transfer };
        bool const is_exclusive{ descriptor.sharing_mode == sharing_mode::exclusive };
        bool const is_cube{ descriptor.type == image::type::cube };
        std::uint32_t const array_layers{ is_cube ? descriptor.array_count * 6u : descriptor.array_count };

        VkImageCreateInfo const create_info{
            .sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext                 = nullptr,
            .flags                 = is_cube ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u,
            .imageType             = vulkan_image::convert_type(descriptor.type),
            .format                = vulkan_image::convert_format(descriptor.format),
            .extent                = { descriptor.dimensions.x, descriptor.dimensions.y, 1 },
            .mipLevels             = 1,
            .arrayLayers           = array_layers,
            .samples               = VK_SAMPLE_COUNT_1_BIT,
            .tiling                = VK_IMAGE_TILING_OPTIMAL,
            .usage                 = vulkan_image::convert_usage(descriptor.usage_flags),
            .sharingMode           = is_exclusive ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = is_exclusive ? 0 : static_cast<std::uint32_t>(shared_queue_indices.size()),
            .pQueueFamilyIndices   = is_exclusive ? nullptr : shared_queue_indices.data(),
            .initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED,
        };

        VkImage image_handle{ VK_NULL_HANDLE };
        EMBER_VULKAN_VERIFY_RESULT(vkCreateImage(device, &create_info, &global_host_allocation_callbacks, &image_handle), "Failed to create VkImage.");

        VkMemoryRequirements memory_requirements{};
        vkGetImageMemoryRequirements(device, image_handle, &memory_requirements);

        device_memory_allocator::chunk const *allocated_chunk{ memory_allocator->alloc(memory_requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) };
        EMBER_THROW_IF_FAILED(allocated_chunk != nullptr, resource_creation_exception{ "Failed to create VkImage. Could not allocate any device memory." });
        vkBindImageMemory(device, image_handle, allocated_chunk->memory, allocated_chunk->offset);

        SET_RESOURCE_NAME(image_handle, VK_OBJECT_TYPE_IMAGE, name.data());

        return make_unique<vulkan_image>(descriptor, device, image_handle, memory_allocator, allocated_chunk);
    }

    unique_ptr<image_view> vulkan_resource_factory::create_image_view(image const &image, image_view::descriptor const &descriptor, std::string_view name) const {
        image::descriptor const &image_descriptor{ image.get_descriptor() };
        VkImageAspectFlags const aspect_flags{ static_cast<VkImageAspectFlags>(image_descriptor.format == image::format::D32_SFLOAT ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT) };

        VkImageViewCreateInfo const create_info{
            .sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext      = nullptr,
            .flags      = 0,
            .image      = resource_cast<vulkan_image const>(&image)->get_image(),
            .viewType   = vulkan_image_view::convert_type(descriptor.type),
            .format     = vulkan_image::convert_format(image_descriptor.format),
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
            },
            .subresourceRange = {
                .aspectMask     = aspect_flags,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = descriptor.layer,
                .layerCount     = descriptor.layer_count,
            },
        };

        VkImageView image_view_handle{ VK_NULL_HANDLE };
        EMBER_VULKAN_VERIFY_RESULT(vkCreateImageView(device, &create_info, &global_host_allocation_callbacks, &image_view_handle), "Failed to create VkImageView");

        return make_unique<vulkan_image_view>(device, image_view_handle);
    }

    unique_ptr<graphics_pipeline_object> vulkan_resource_factory::create_graphics_pipeline_object(graphics_pipeline_object::descriptor const &descriptor, std::string_view name) const {
        //Descriptor set layouts
        array<VkDescriptorSetLayout> descriptor_layouts(descriptor.descriptor_set_layouts.size());
        for(std::size_t i{ 0 }; i < descriptor_layouts.size(); ++i) {
            descriptor_layouts[i] = resource_cast<vulkan_descriptor_set_layout const>(descriptor.descriptor_set_layouts[i])->get_layout();
        }

        //Push constants
        array<VkPushConstantRange> push_constant_ranges(descriptor.push_constants.size());
        for(std::size_t i{ 0 }; i < push_constant_ranges.size(); ++i) {
            auto const &push_constant{ descriptor.push_constants[i] };
            push_constant_ranges[i] = VkPushConstantRange{
                .stageFlags = vulkan_shader::convert_stage(push_constant.stage),
                .offset     = static_cast<std::uint32_t>(push_constant.offset),
                .size       = static_cast<std::uint32_t>(push_constant.bytes),
            };
        }

        //Pipeline layout
        VkPipelineLayoutCreateInfo const pipeline_layout_info{
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .setLayoutCount         = static_cast<std::uint32_t>(descriptor_layouts.size()),
            .pSetLayouts            = descriptor_layouts.data(),
            .pushConstantRangeCount = static_cast<std::uint32_t>(push_constant_ranges.size()),
            .pPushConstantRanges    = push_constant_ranges.data(),
        };

        VkPipelineLayout pipeline_layout{ VK_NULL_HANDLE };
        EMBER_VULKAN_VERIFY_RESULT(vkCreatePipelineLayout(device, &pipeline_layout_info, &global_host_allocation_callbacks, &pipeline_layout), "Failed to create VkPipelineLayout.");

        //TODO: custom static array
        std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages{};

        //Vertex shader
        shader_stages[0] = VkPipelineShaderStageCreateInfo{
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext  = nullptr,
            .flags  = 0,
            .stage  = VK_SHADER_STAGE_VERTEX_BIT,
            .module = resource_cast<vulkan_shader const>(descriptor.vertex_shader)->get_module(),
            .pName  = "main",
        };

        //Pixel shader
        shader_stages[1] = VkPipelineShaderStageCreateInfo{
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext  = nullptr,
            .flags  = 0,
            .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = resource_cast<vulkan_shader const>(descriptor.pixel_shader)->get_module(),
            .pName  = "main",
        };

        //Vertex input
        VkVertexInputBindingDescription const input_binding_description{
            .binding   = 0,
            .stride    = descriptor.vertex_input.stride,
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        };

        array<VkVertexInputAttributeDescription> vertex_attributes(descriptor.vertex_attributes.size());
        for(std::size_t i{ 0 }; i < vertex_attributes.size(); ++i) {
            auto const &attribute{ descriptor.vertex_attributes[i] };
            vertex_attributes[i] = VkVertexInputAttributeDescription{
                .location = static_cast<std::uint32_t>(i),
                .binding  = 0,
                .format   = vulkan_graphics_pipeline_object::convert_attribute_format(attribute.format),
                .offset   = attribute.offset,
            };
        }

        VkPipelineVertexInputStateCreateInfo const vertex_input_info{
            .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext                           = nullptr,
            .flags                           = 0,
            .vertexBindingDescriptionCount   = 1,
            .pVertexBindingDescriptions      = &input_binding_description,
            .vertexAttributeDescriptionCount = static_cast<std::uint32_t>(vertex_attributes.size()),
            .pVertexAttributeDescriptions    = vertex_attributes.data(),
        };

        //Input assembly
        VkPipelineInputAssemblyStateCreateInfo const input_assembly_info{
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE,
        };

        //Viewport / Scissor
        array<VkViewport> viewport_states{};
        array<VkRect2D> scissor_states{};
        array<VkDynamicState> dynamic_states{};

        if(descriptor.viewport.state == element_state::fixed) {
            viewport_states.emplace_back(VkViewport{
                .x        = static_cast<float>(descriptor.viewport.position.x),
                .y        = static_cast<float>(descriptor.viewport.position.y),
                .width    = static_cast<float>(descriptor.viewport.size.x),
                .height   = static_cast<float>(descriptor.viewport.size.y),
                .minDepth = 0.0f,
                .maxDepth = 1.0f,
            });
        } else {
            dynamic_states.push_back(VK_DYNAMIC_STATE_VIEWPORT);
        }

        if(descriptor.viewport.state == element_state::fixed) {
            scissor_states.emplace_back(VkRect2D{
                .offset = { descriptor.scissor.position.x, descriptor.scissor.position.y },
                .extent = { descriptor.scissor.size.x, descriptor.scissor.size.y },
            });
        } else {
            dynamic_states.push_back(VK_DYNAMIC_STATE_SCISSOR);
        }

        VkPipelineViewportStateCreateInfo const viewport_state_info{
            .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext         = nullptr,
            .flags         = 0,
            .viewportCount = 1,//Dynamic or not vulkan expects 1 viewport to be bound
            .pViewports    = viewport_states.data(),
            .scissorCount  = 1,//Dynamic or not vulkan expects 1 scissor rect to be bound
            .pScissors     = scissor_states.data(),
        };

        VkPipelineDynamicStateCreateInfo const dynamic_viewport_state_info{
            .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .pNext             = nullptr,
            .flags             = 0,
            .dynamicStateCount = static_cast<std::uint32_t>(dynamic_states.size()),
            .pDynamicStates    = dynamic_states.data(),
        };

        //Rasteriser
        VkPipelineRasterizationStateCreateInfo const resteriser_info{
            .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0,
            .depthClampEnable        = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode             = VK_POLYGON_MODE_FILL,
            .cullMode                = VK_CULL_MODE_BACK_BIT,
            .frontFace               = VK_FRONT_FACE_CLOCKWISE,
            .depthBiasEnable         = VK_FALSE,
            .depthBiasConstantFactor = 0.0f,
            .depthBiasClamp          = 0.0f,
            .depthBiasSlopeFactor    = 0.0f,
            .lineWidth               = 1.0f,//Any thicker requires the 'wide lines' feature
        };

        //Multisampling
        VkPipelineMultisampleStateCreateInfo const multisample_info{
            .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext                 = nullptr,
            .flags                 = 0,
            .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable   = VK_FALSE,
            .minSampleShading      = 0.0f,
            .pSampleMask           = nullptr,
            .alphaToCoverageEnable = VK_FALSE,
            .alphaToOneEnable      = VK_FALSE,
        };

        //Depth / Stencil
        VkPipelineDepthStencilStateCreateInfo const depth_stencil_info{
            .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .pNext                 = nullptr,
            .flags                 = 0,
            .depthTestEnable       = static_cast<VkBool32>(descriptor.depth_state.depth_test ? VK_TRUE : VK_FALSE),
            .depthWriteEnable      = static_cast<VkBool32>(descriptor.depth_state.depth_write ? VK_TRUE : VK_FALSE),
            .depthCompareOp        = VK_COMPARE_OP_LESS,
            .depthBoundsTestEnable = VK_FALSE,
            .stencilTestEnable     = VK_FALSE,
            .front                 = {},
            .back                  = {},
            .minDepthBounds        = 0.0f,
            .maxDepthBounds        = 1.0f,
        };

        //Colour blending
        VkPipelineColorBlendAttachmentState const colour_blend_attachment{
            .blendEnable         = static_cast<VkBool32>(descriptor.enable_blending ? VK_TRUE : VK_FALSE),
            .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            .colorBlendOp        = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp        = VK_BLEND_OP_ADD,
            .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        };

        VkPipelineColorBlendStateCreateInfo const colour_blend_info{
            .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable   = VK_FALSE,
            .logicOp         = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments    = &colour_blend_attachment,
            .blendConstants  = { 0.0f, 0.0f, 0.0f, 0.0f },
        };

        //Pipeline
        VkGraphicsPipelineCreateInfo const pipeline_info{
            .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext               = nullptr,
            .flags               = 0,
            .stageCount          = shader_stages.size(),
            .pStages             = shader_stages.data(),
            .pVertexInputState   = &vertex_input_info,
            .pInputAssemblyState = &input_assembly_info,
            .pTessellationState  = nullptr,
            .pViewportState      = &viewport_state_info,
            .pRasterizationState = &resteriser_info,
            .pMultisampleState   = &multisample_info,
            .pDepthStencilState  = &depth_stencil_info,
            .pColorBlendState    = &colour_blend_info,
            .pDynamicState       = &dynamic_viewport_state_info,
            .layout              = pipeline_layout,
            .renderPass          = resource_cast<vulkan_render_pass const>(descriptor.render_pass)->get_render_pass(),
            .subpass             = 0,
            .basePipelineHandle  = VK_NULL_HANDLE,
            .basePipelineIndex   = -1,
        };

        VkPipeline pipeline{ VK_NULL_HANDLE };
        EMBER_VULKAN_VERIFY_RESULT(vkCreateGraphicsPipelines(device, nullptr, 1, &pipeline_info, &global_host_allocation_callbacks, &pipeline), "Failed to create VkPipeline.");

        SET_RESOURCE_NAME(pipeline, VK_OBJECT_TYPE_PIPELINE, name.data());

        return make_unique<vulkan_graphics_pipeline_object>(device, pipeline, pipeline_layout);
    }

    unique_ptr<descriptor_set_layout> vulkan_resource_factory::create_descriptor_set_layout(descriptor_set_layout::descriptor const &descriptor, std::string_view name) const {
        array<VkDescriptorSetLayoutBinding> layout_bindings(descriptor.bindings.size());
        for(std::size_t i{ 0 }; i < layout_bindings.size(); ++i) {
            auto const &binding{ descriptor.bindings[i] };
            layout_bindings[i] = VkDescriptorSetLayoutBinding{
                .binding            = binding.binding,
                .descriptorType     = vulkan_descriptor_set_layout::convert_descriptor_type(binding.type),
                .descriptorCount    = static_cast<std::uint32_t>(binding.array_size),
                .stageFlags         = vulkan_shader::convert_stage(binding.stage),
                .pImmutableSamplers = nullptr,
            };
        }

        VkDescriptorSetLayoutCreateInfo const create_info{
            .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext        = nullptr,
            .flags        = 0,
            .bindingCount = static_cast<std::uint32_t>(layout_bindings.size()),
            .pBindings    = layout_bindings.data(),
        };

        VkDescriptorSetLayout layout{ VK_NULL_HANDLE };
        EMBER_VULKAN_VERIFY_RESULT(vkCreateDescriptorSetLayout(device, &create_info, &global_host_allocation_callbacks, &layout), "Failed to create VkDescriptorSetLayout.");

        SET_RESOURCE_NAME(layout, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, name.data());

        return make_unique<vulkan_descriptor_set_layout>(device, layout);
    }

    unique_ptr<render_pass> vulkan_resource_factory::create_render_pass(render_pass::descriptor const &descriptor, std::string_view name) const {
        //Convert attachments
        std::size_t const colour_attachment_size{ descriptor.colour_attachments.size() };
        array<VkAttachmentDescription> attachments(colour_attachment_size);
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

        array<VkAttachmentReference> colour_attachment_references(colour_attachment_size);
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