#include "vulkan_queue.hpp"

#include "commands.hpp"
#include "ember/graphics/command_buffer.hpp"
#include "host_memory_allocator.hpp"
#include "memory_barrier_simplification.hpp"
#include "resource_cast.hpp"
#include "verification.hpp"
#include "vulkan_buffer.hpp"
#include "vulkan_descriptor_set.hpp"
#include "vulkan_extension_functions.hpp"
#include "vulkan_fence.hpp"
#include "vulkan_framebuffer.hpp"
#include "vulkan_graphics_pipeline_object.hpp"
#include "vulkan_render_pass.hpp"
#include "vulkan_semaphore.hpp"
#include "vulkan_shader.hpp"
#include "vulkan_swapchain.hpp"

#include <ember/containers/array.hpp>
#include <ember/core/log.hpp>
#include <ember/core/profiling.hpp>

using namespace ember::containers;

namespace {
    VkIndexType get_index_type(ember::graphics::index_type const type) {
        switch(type) {
            case ember::graphics::index_type::uint16:
                return VK_INDEX_TYPE_UINT16;
            default:
                EMBER_CHECK(false);
                return VK_INDEX_TYPE_UINT16;
        }
    }

    VkPipelineStageFlags convert_stage(ember::graphics::pipeline_stage const stage) {
        switch(stage) {
            case ember::graphics::pipeline_stage::top:
                return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            case ember::graphics::pipeline_stage::transfer:
                return VK_PIPELINE_STAGE_TRANSFER_BIT;
            case ember::graphics::pipeline_stage::compute_shader:
                return VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            case ember::graphics::pipeline_stage::vertex_input:
                return VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
            case ember::graphics::pipeline_stage::vertex_shader:
                return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
            case ember::graphics::pipeline_stage::pixel_shader:
                return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            case ember::graphics::pipeline_stage::early_pixel_test:
                return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            case ember::graphics::pipeline_stage::late_pixel_test:
                return VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            case ember::graphics::pipeline_stage::colour_attachment_output:
                return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            default:
                EMBER_CHECK(false);
                return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        }
    }

    template<typename... Ts>
    struct match : Ts... { using Ts::operator()...; };
}

namespace ember::graphics {
    vulkan_queue::vulkan_queue(VkPhysicalDevice physical_device, VkDevice logical_device, queue_family_indices family_indices)
        : physical_device{ physical_device }
        , logical_device{ logical_device }
        , family_indices{ family_indices } {
        graphics_queue = create_queue(family_indices.graphics, "Graphics Queue");
        compute_queue  = create_queue(family_indices.compute, "Compute Queue");
        transfer_queue = create_queue(family_indices.transfer, "Transfer Queue");
    }

    vulkan_queue::~vulkan_queue() = default;

    void vulkan_queue::submit(graphics_submit_info const &submit_info, fence const *const signal_fence) {
        submit_to_queue(graphics_queue, VK_PIPELINE_BIND_POINT_GRAPHICS, submit_info, signal_fence);
    }

    void vulkan_queue::submit(compute_submit_info const &submit_info, fence const *const signal_fence) {
        submit_to_queue(graphics_queue, VK_PIPELINE_BIND_POINT_COMPUTE, submit_info, signal_fence);
    }

    void vulkan_queue::submit(transfer_submit_info const &submit_info, fence const *const signal_fence) {
        submit_to_queue(graphics_queue, static_cast<VkPipelineBindPoint>(0), submit_info, signal_fence);//NOTE: No bind point available for transfer
    }

    swapchain::result vulkan_queue::present(swapchain const *const swapchain, std::size_t const image_index, semaphore const *const wait_semaphore) {
        VkSwapchainKHR const swapchain_handle{ resource_cast<vulkan_swapchain const>(swapchain)->get_handle() };
        VkSemaphore const semaphore_handle{ resource_cast<vulkan_semaphore const>(wait_semaphore)->get_handle() };

        auto const index{ static_cast<std::uint32_t>(image_index) };

        VkPresentInfoKHR const present_submit_info{
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext              = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores    = &semaphore_handle,
            .swapchainCount     = 1,
            .pSwapchains        = &swapchain_handle,
            .pImageIndices      = &index,
        };

        return vulkan_swapchain::convert_result(vkQueuePresentKHR(graphics_queue.handle, &present_submit_info));
    }

    void vulkan_queue::release_resources() {
        destroy_queue(graphics_queue);
        destroy_queue(compute_queue);
        destroy_queue(transfer_queue);
    }

    void vulkan_queue::record_commands(queue &queue, VkPipelineBindPoint const bind_point, VkCommandBuffer vk_cmd_buffer, command_buffer const &command_buffer) {
        EMBER_PROFILE_FUNCTION;

        VkPipelineLayout current_pipeline_layout{ VK_NULL_HANDLE };

        for(auto &&[type, command_memory] : command_buffer) {
            switch(type) {
#if EMBER_GRAPHICS_ENABLE_USER_MARKERS
                case command_type::push_user_marker_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::push_user_marker_command> *>(command_memory) };
    #if EMBER_GRAPHICS_DEBUG_UTILITIES
                    VkDebugUtilsLabelEXT const label{
                        .sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
                        .pNext      = nullptr,
                        .pLabelName = command->name.c_str(),
                        .color      = { command->colour.r, command->colour.g, command->colour.b, command->colour.a },
                    };

                    fp_vkCmdBeginDebugUtilsLabelEXT(vk_cmd_buffer, &label);
    #endif
    #if EMBER_CORE_ENABLE_PROFILING
                    //TODO: Colour zones
                    if(!queue.source_datas.contains(command->name)) {
                        queue.source_datas[command->name] = tracy::SourceLocationData{ command->name.c_str(), __FUNCTION__, __FILE__, (uint32_t)__LINE__, 0 };
                    }
                    queue.scoped_events.emplace(queue.profiling_context, &queue.source_datas.at(command->name), vk_cmd_buffer, true);
    #endif
                } break;
                case command_type::pop_user_marker_command:
    #if EMBER_GRAPHICS_DEBUG_UTILITIES
                    fp_vkCmdEndDebugUtilsLabelEXT(vk_cmd_buffer);
    #endif
    #if EMBER_CORE_ENABLE_PROFILING
                    queue.scoped_events.pop();
    #endif
                    break;
#endif
                case command_type::copy_buffer_to_buffer_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::copy_buffer_to_buffer_command> *>(command_memory) };

                    VkBufferCopy const copy_region{
                        .srcOffset = command->source_offset,
                        .dstOffset = command->destination_offset,
                        .size      = command->bytes,
                    };

                    VkBuffer source{ resource_cast<vulkan_buffer const>(command->source)->get_handle() };
                    VkBuffer destination{ resource_cast<vulkan_buffer const>(command->destination)->get_handle() };

                    vkCmdCopyBuffer(vk_cmd_buffer, source, destination, 1, &copy_region);
                } break;
                case command_type::copy_buffer_to_image_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::copy_buffer_to_image_command> *>(command_memory) };

                    VkBufferImageCopy const copy_region{
                        .bufferOffset      = command->source_offset,
                        .bufferRowLength   = 0,//Tightly packed
                        .bufferImageHeight = 0,//Tightly packed
                        .imageSubresource  = {
                             .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,//TODO: Handle other aspect masks
                             .mipLevel       = 0,
                             .baseArrayLayer = command->destination_base_layer,
                             .layerCount     = command->destination_layer_count,
                        },
                        .imageOffset = { static_cast<std::int32_t>(command->destination_offset.x), static_cast<std::int32_t>(command->destination_offset.y), static_cast<std::int32_t>(command->destination_offset.z) },
                        .imageExtent = { command->destination_extent.x, command->destination_extent.y, command->destination_extent.z },
                    };

                    VkBuffer source{ resource_cast<vulkan_buffer const>(command->source)->get_handle() };
                    VkImage destination{ resource_cast<vulkan_image const>(command->destination)->get_handle() };

                    vkCmdCopyBufferToImage(vk_cmd_buffer, source, destination, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);
                } break;
                case command_type::copy_image_to_buffer_command:
                    break;
                case command_type::execution_barrier_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::execution_barrier_command> *>(command_memory) };

                    VkPipelineStageFlags const source_stage{ convert_stage(command->source_stage) };
                    VkPipelineStageFlags const destination_stage{ convert_stage(command->destination_stage) };

                    vkCmdPipelineBarrier(vk_cmd_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 0, nullptr);
                } break;
                case command_type::buffer_memory_barrier_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::buffer_memory_barrier_command> *>(command_memory) };

                    buffer_access_info const previous_access_info{ get_buffer_access(command->barrier_info.previous_access) };
                    buffer_access_info const next_access_info{ get_buffer_access(command->barrier_info.next_access) };

                    std::uint32_t const source_queue_index{ get_queue_family_index(command->barrier_info.source_queue) };
                    std::uint32_t const destination_queue_index{ get_queue_family_index(command->barrier_info.destination_queue) };

                    VkBufferMemoryBarrier const barrier{
                        .sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
                        .pNext               = nullptr,
                        .srcAccessMask       = previous_access_info.access_mask,
                        .dstAccessMask       = next_access_info.access_mask,
                        .srcQueueFamilyIndex = source_queue_index,
                        .dstQueueFamilyIndex = destination_queue_index,
                        .buffer              = resource_cast<vulkan_buffer const>(command->buffer)->get_handle(),
                        .offset              = 0,
                        .size                = VK_WHOLE_SIZE,
                    };

                    vkCmdPipelineBarrier(vk_cmd_buffer, previous_access_info.stage_mask, next_access_info.stage_mask, 0, 0, nullptr, 1, &barrier, 0, nullptr);

                } break;
                case command_type::image_memory_barrier_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::image_memory_barrier_command> *>(command_memory) };

                    image_access_info const previous_access_info{ get_image_access(command->barrier_info.previous_access) };
                    image_access_info const next_access_info{ get_image_access(command->barrier_info.next_access) };

                    std::uint32_t const source_queue_index{ get_queue_family_index(command->barrier_info.source_queue) };
                    std::uint32_t const destination_queue_index{ get_queue_family_index(command->barrier_info.destination_queue) };

                    VkImageAspectFlags const aspect_flags{ static_cast<VkImageAspectFlags>(command->image->get_descriptor().format == image::format::D32_SFLOAT ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT) };

                    VkImageMemoryBarrier const barrier{
                        .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                        .pNext               = nullptr,
                        .srcAccessMask       = previous_access_info.access_mask,
                        .dstAccessMask       = next_access_info.access_mask,
                        .oldLayout           = previous_access_info.image_layout,
                        .newLayout           = next_access_info.image_layout,
                        .srcQueueFamilyIndex = source_queue_index,
                        .dstQueueFamilyIndex = destination_queue_index,
                        .image               = resource_cast<vulkan_image const>(command->image)->get_handle(),
                        .subresourceRange    = {
                               .aspectMask     = aspect_flags,
                               .baseMipLevel   = 0,
                               .levelCount     = 1,
                               .baseArrayLayer = command->barrier_info.base_layer,
                               .layerCount     = command->barrier_info.layer_count,
                        },
                    };

                    vkCmdPipelineBarrier(vk_cmd_buffer, previous_access_info.stage_mask, next_access_info.stage_mask, 0, 0, nullptr, 0, nullptr, 1, &barrier);

                } break;
                case command_type::bind_compute_pipeline_object_command:
                    break;
                case command_type::bind_descriptor_set_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::bind_descriptor_set_command> *>(command_memory) };

                    VkDescriptorSet const descriptor_set_handle{ resource_cast<vulkan_descriptor_set const>(command->descriptor_set)->get_handle() };

                    vkCmdBindDescriptorSets(vk_cmd_buffer, bind_point, current_pipeline_layout, command->set_num, 1, &descriptor_set_handle, 0, nullptr);
                } break;
                case command_type::push_constant_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::push_constant_command> *>(command_memory) };

                    vkCmdPushConstants(vk_cmd_buffer, current_pipeline_layout, vulkan_shader::convert_stage(command->stage), command->offset, command->bytes, command->data);
                } break;
                case command_type::dispatch_command:
                    break;
                case command_type::dispatch_indirect_command:
                    break;
                case command_type::begin_render_pass_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::begin_render_pass_command> *>(command_memory) };

                    array<VkClearValue> clear_values(command->clear_values.size());
                    for(std::size_t i{ 0 }; i < command->clear_values.size(); ++i) {
                        std::visit(match{
                                       [&](colour_value const colour) { clear_values[i].color = { colour.r, colour.g, colour.b, colour.a }; },
                                       [&](depth_stencil_value const depth_stencil) { clear_values[i].depthStencil = { depth_stencil.depth, depth_stencil.stencil }; },
                                   },
                                   command->clear_values[i]);
                    }

                    VkRenderPassBeginInfo const render_pass_info{
                        .sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                        .pNext       = nullptr,
                        .renderPass  = resource_cast<vulkan_render_pass const>(command->render_pass)->get_handle(),
                        .framebuffer = resource_cast<vulkan_framebuffer const>(command->framebuffer)->get_handle(),
                        .renderArea  = {
                             .offset = { command->render_area.origin.x, command->render_area.origin.y },
                             .extent = { command->render_area.size.x, command->render_area.size.y },
                        },
                        .clearValueCount = static_cast<std::uint32_t>(clear_values.size()),
                        .pClearValues    = clear_values.data(),
                    };

                    vkCmdBeginRenderPass(vk_cmd_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
                } break;
                case command_type::end_render_pass_command:
                    vkCmdEndRenderPass(vk_cmd_buffer);
                    break;
                case command_type::set_viewport_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::set_viewport_command> *>(command_memory) };

                    VkViewport const viewport{
                        .x        = static_cast<float>(command->position.x),
                        .y        = static_cast<float>(command->position.y),
                        .width    = static_cast<float>(command->size.x),
                        .height   = static_cast<float>(command->size.y),
                        .minDepth = 0.0f,
                        .maxDepth = 1.0f,
                    };

                    vkCmdSetViewport(vk_cmd_buffer, 0, 1, &viewport);
                } break;
                case command_type::set_scissor_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::set_scissor_command> *>(command_memory) };

                    VkRect2D const scissor{
                        .offset = {
                            .x = command->position.x,
                            .y = command->position.y,
                        },
                        .extent = {
                            .width  = command->size.x,
                            .height = command->size.y,
                        },
                    };

                    vkCmdSetScissor(vk_cmd_buffer, 0, 1, &scissor);
                } break;
                case command_type::bind_graphics_pipeline_object_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::bind_graphics_pipeline_object_command> *>(command_memory) };

                    auto const *pipeline{ resource_cast<vulkan_graphics_pipeline_object const>(command->pipeline_object) };

                    current_pipeline_layout = pipeline->get_layout_handle();
                    vkCmdBindPipeline(vk_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->get_pipeline_handle());
                } break;
                case command_type::bind_vertex_buffer_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::bind_vertex_buffer_command> *>(command_memory) };

                    VkBuffer const buffer_handle{ resource_cast<vulkan_buffer const>(command->vertex_buffer)->get_handle() };

                    vkCmdBindVertexBuffers(vk_cmd_buffer, 0, 1, &buffer_handle, &command->offset);
                } break;
                case command_type::bind_index_buffer_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::bind_index_buffer_command> *>(command_memory) };

                    VkBuffer const buffer_handle{ resource_cast<vulkan_buffer const>(command->index_buffer)->get_handle() };

                    vkCmdBindIndexBuffer(vk_cmd_buffer, buffer_handle, command->offset, get_index_type(command->index_type));
                } break;
                case command_type::draw_indexed_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::draw_indexed_command> *>(command_memory) };

                    vkCmdDrawIndexed(vk_cmd_buffer, command->index_count, command->instance_count, command->first_index, command->vertex_offset, command->first_instance);
                } break;
                case command_type::draw_indexed_indirect_command: {
                    auto *command{ reinterpret_cast<recorded_command<command_type::draw_indexed_indirect_command> *>(command_memory) };

                    VkBuffer const buffer_handle{ resource_cast<vulkan_buffer const>(command->indirect_buffer)->get_handle() };

                    vkCmdDrawIndexedIndirect(vk_cmd_buffer, buffer_handle, command->offset, command->draw_count, sizeof(draw_indexed_indirect_info));
                } break;
                default:
                    EMBER_CHECK(false);
                    break;
            }
        }
    }

    vulkan_queue::queue vulkan_queue::create_queue(std::uint32_t const family_index, std::string_view name) {
        VkCommandPoolCreateInfo const command_pool_create_info{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = family_index,
        };

        queue queue{};
        vkGetDeviceQueue(logical_device, family_index, 0, &queue.handle);
        EMBER_VULKAN_VERIFY_RESULT(vkCreateCommandPool(logical_device, &command_pool_create_info, &global_host_allocation_callbacks, &queue.command_pool), "Failed to create VkCommandPool.");

#if EMBER_CORE_ENABLE_PROFILING
        if(family_index != family_indices.transfer) {
            VkCommandBuffer init_buff{ VK_NULL_HANDLE };
            VkCommandBufferAllocateInfo const buffer_alloc_info{
                .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .pNext              = nullptr,
                .commandPool        = queue.command_pool,
                .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount = 1,
            };
            vkAllocateCommandBuffers(logical_device, &buffer_alloc_info, &init_buff);
            queue.profiling_context = TracyVkContextCalibrated(physical_device, logical_device, queue.handle, init_buff, fp_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT, fp_vkGetCalibratedTimestampsEXT);
            vkFreeCommandBuffers(logical_device, queue.command_pool, 1, &init_buff);
            TracyVkContextName(queue.profiling_context, name.data(), name.size());
        }
#endif

        return queue;
    }

    template<typename submit_info_t>
    void vulkan_queue::submit_to_queue(queue &queue, VkPipelineBindPoint const bind_point, submit_info_t const &submit_info, fence const *const signal_fence) {
        EMBER_PROFILE_FUNCTION;

        reset_available_buffers(queue);

        VkCommandBufferBeginInfo const begin_info{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags            = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            .pInheritanceInfo = nullptr,
        };

        //Record all command buffers
        containers::array<VkCommandBuffer> recorded_command_buffers{};

        for(auto const *command_buffer : submit_info.command_buffers) {
            VkCommandBuffer vk_command_buffer{ alloc_buffer(queue) };

            EMBER_VULKAN_VERIFY_RESULT(vkBeginCommandBuffer(vk_command_buffer, &begin_info), "Failed to begin recording.");

            record_commands(queue, bind_point, vk_command_buffer, *command_buffer);
#if EMBER_CORE_ENABLE_PROFILING
            if(queue.profiling_context != nullptr) {
                TracyVkCollect(queue.profiling_context, vk_command_buffer);
            }
#endif

            EMBER_VULKAN_VERIFY_RESULT(vkEndCommandBuffer(vk_command_buffer), "Failed to end recording.");

            recorded_command_buffers.push_back(vk_command_buffer);
        }

        //Do the actual queue submission
        containers::array<VkSemaphore> wait_semaphores{};
        containers::array<VkPipelineStageFlags> wait_stages{};
        std::size_t const wait_semaphore_count{ submit_info.wait_semaphores.size() };
        wait_semaphores.resize(wait_semaphore_count);
        wait_stages.resize(wait_semaphore_count);
        for(std::size_t i{ 0 }; i < wait_semaphore_count; ++i) {
            wait_semaphores[i] = resource_cast<vulkan_semaphore const>(submit_info.wait_semaphores[i].first)->get_handle();
            wait_stages[i]     = convert_stage(submit_info.wait_semaphores[i].second);
        }

        containers::array<VkSemaphore> signal_semaphores{};
        std::size_t const signal_semaphore_count{ submit_info.signal_semaphores.size() };
        signal_semaphores.resize(signal_semaphore_count);
        for(std::size_t i{ 0 }; i < signal_semaphore_count; ++i) {
            signal_semaphores[i] = resource_cast<vulkan_semaphore const>(submit_info.signal_semaphores[i])->get_handle();
        }

        VkSubmitInfo const graphics_submit_info{
            .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                = nullptr,
            .waitSemaphoreCount   = static_cast<std::uint32_t>(wait_semaphore_count),
            .pWaitSemaphores      = wait_semaphores.data(),
            .pWaitDstStageMask    = wait_stages.data(),
            .commandBufferCount   = static_cast<std::uint32_t>(recorded_command_buffers.size()),
            .pCommandBuffers      = recorded_command_buffers.data(),
            .signalSemaphoreCount = static_cast<std::uint32_t>(signal_semaphore_count),
            .pSignalSemaphores    = signal_semaphores.data(),
        };

        VkFence const fence_handle{ signal_fence != nullptr ? resource_cast<vulkan_fence const>(signal_fence)->get_handle() : VK_NULL_HANDLE };
        EMBER_VULKAN_VERIFY_RESULT(vkQueueSubmit(queue.handle, 1, &graphics_submit_info, fence_handle), "Failed to submit user batch.");

        //Insert our own fence right after this queue so we know when the buffers are free to use again.
        VkFence buffer_fence{ VK_NULL_HANDLE };

        if(!queue.pooled_fences.empty()) {
            buffer_fence = queue.pooled_fences.back();
            queue.pooled_fences.pop_back();
        } else {
            VkFenceCreateInfo const create_info{
                .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0u,
            };
            EMBER_VULKAN_VERIFY_RESULT(vkCreateFence(logical_device, &create_info, &global_host_allocation_callbacks, &buffer_fence), "Failed to create VkFence.");
        }

        EMBER_VULKAN_VERIFY_RESULT(vkQueueSubmit(queue.handle, 0, nullptr, buffer_fence), "Failed to pool fence.");

        queue.pending_buffers.emplace_back(std::move(recorded_command_buffers), buffer_fence);
    }

    void vulkan_queue::destroy_queue(queue &queue) {
        vkQueueWaitIdle(queue.handle);

        if(queue.pooled_buffers.size() > 0) {
            vkFreeCommandBuffers(logical_device, queue.command_pool, queue.pooled_buffers.size(), queue.pooled_buffers.data());
        }
        for(auto *fence : queue.pooled_fences) {
            vkDestroyFence(logical_device, fence, &global_host_allocation_callbacks);
        }

        for(auto &pending : queue.pending_buffers) {
            vkFreeCommandBuffers(logical_device, queue.command_pool, pending.buffers.size(), pending.buffers.data());
            vkDestroyFence(logical_device, pending.fence, &global_host_allocation_callbacks);
        }

        vkDestroyCommandPool(logical_device, queue.command_pool, &global_host_allocation_callbacks);

#if EMBER_CORE_ENABLE_PROFILING
        if(queue.profiling_context != nullptr) {
            TracyVkDestroy(queue.profiling_context);
        }
#endif
    }

    std::uint32_t vulkan_queue::get_queue_family_index(queue_type const queue_type) {
        switch(queue_type) {
            case queue_type::none:
                return VK_QUEUE_FAMILY_IGNORED;
            case queue_type::graphics:
                return family_indices.graphics;
            case queue_type::compute:
                return family_indices.compute;
            case queue_type::transfer:
                return family_indices.transfer;
            default:
                EMBER_CHECK(false);
                return VK_QUEUE_FAMILY_IGNORED;
        }
    }

    void vulkan_queue::reset_available_buffers(queue &queue) {
        EMBER_PROFILE_FUNCTION;

        auto const are_buffers_finished = [&](pending_buffer_info const &info) {
            return vkGetFenceStatus(logical_device, info.fence) == VK_SUCCESS;
        };

        array<VkFence> fences_to_reset{};//Reset these after erasing otherwise we will never remove them from the loop.

        for(auto &pending_buffer : queue.pending_buffers) {
            if(are_buffers_finished(pending_buffer)) {
                //TODO: Use insert on array
                for(auto *buff : pending_buffer.buffers) {
                    vkResetCommandBuffer(buff, 0);
                    queue.pooled_buffers.emplace_back(buff);
                }

                queue.pooled_fences.emplace_back(pending_buffer.fence);
                fences_to_reset.emplace_back(pending_buffer.fence);
            }
        }

        queue.pending_buffers.erase(std::remove_if(queue.pending_buffers.begin(), queue.pending_buffers.end(), are_buffers_finished), queue.pending_buffers.end());

        if(!fences_to_reset.empty()) {
            vkResetFences(logical_device, fences_to_reset.size(), fences_to_reset.data());
        }
    }

    VkCommandBuffer vulkan_queue::alloc_buffer(queue &queue) {
        VkCommandBuffer buff{ VK_NULL_HANDLE };

        if(!queue.pooled_buffers.empty()) {
            buff = queue.pooled_buffers.back();
            queue.pooled_buffers.pop_back();
        } else {
            VkCommandBufferAllocateInfo const buffer_alloc_info{
                .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .pNext              = nullptr,
                .commandPool        = graphics_queue.command_pool,
                .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount = 1,
            };
            EMBER_VULKAN_VERIFY_RESULT(vkAllocateCommandBuffers(logical_device, &buffer_alloc_info, &buff), "Failed to allocate VkCommandBuffer.");
        }

        EMBER_CHECK(buff != VK_NULL_HANDLE);
        return buff;
    }
}