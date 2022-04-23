#include "vulkan_graphics_queue.hpp"

#include "TracyVulkan.hpp"
#include "commands.hpp"
#include "ember/graphics/command_buffer_iterator.hpp"
#include "resource_cast.hpp"
#include "verification.hpp"
#include "vulkan_buffer.hpp"
#include "vulkan_extension_functions.hpp"
#include "vulkan_fence.hpp"
#include "vulkan_framebuffer.hpp"
#include "vulkan_graphics_pipeline_object.hpp"
#include "vulkan_render_pass.hpp"
#include "vulkan_semaphore.hpp"
#include "vulkan_swapchain.hpp"

#include <ember/containers/array.hpp>
#include <ember/core/log.hpp>

using namespace ember::containers;

namespace {
//TODO: This stuff
#if EMBER_CORE_ENABLE_PROFILING
    PFN_vkGetCalibratedTimestampsEXT fp_vkGetCalibratedTimestampsEXT;
    PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT fp_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;
#endif

    VkIndexType get_index_type(ember::graphics::index_type const type) {
        switch(type) {
            case ember::graphics::index_type::uint16:
                return VK_INDEX_TYPE_UINT16;
            default:
                EMBER_CHECK(false);
                return VK_INDEX_TYPE_UINT16;
        }
    }

    template<typename... Ts>
    struct match : Ts... { using Ts::operator()...; };

    //TODO: Move to common place for other queues
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
}

namespace ember::graphics {
    vulkan_graphics_queue::vulkan_graphics_queue(VkPhysicalDevice physical_device, VkDevice logical_device, VkQueue handle, VkCommandPool command_pool)
        : physical_device{ physical_device }
        , logical_device{ logical_device }
        , handle{ handle }
        , command_pool{ command_pool } {
    }

    void vulkan_graphics_queue::submit(graphics_submit_info const &submit_info, fence const *const signal_fence) {
        VkCommandBufferAllocateInfo const buffer_alloc_info{
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext              = nullptr,
            .commandPool        = command_pool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        VkCommandBuffer vk_command_buffer{ VK_NULL_HANDLE };
        EMBER_VULKAN_VERIFY_RESULT(vkAllocateCommandBuffers(logical_device, &buffer_alloc_info, &vk_command_buffer), "Failed to allocate VkCommandBuffer.");

        TracyVkCtx ctx{ TracyVkContextCalibrated(physical_device, logical_device, handle, vk_command_buffer, , ) };//TODO: calibrated stuff

        {
            VkCommandBufferBeginInfo const begin_info{
                .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .flags            = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
                .pInheritanceInfo = nullptr,
            };
            EMBER_VULKAN_VERIFY_RESULT(vkBeginCommandBuffer(vk_command_buffer, &begin_info), "Failed to begin recording.");
        }

        for(auto const *command_buffer : submit_info.command_buffers) {
            for(auto &&[type, command_memory] : *command_buffer) {
                switch(type) {
#if EMBER_GRAPHICS_DEBUG_UTILITIES
                    case command_type::push_user_marker_command: {
                        auto *command{ reinterpret_cast<recorded_command<command_type::push_user_marker_command> *>(command_memory) };

                        VkDebugUtilsLabelEXT const label{
                            .sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
                            .pNext      = nullptr,
                            .pLabelName = command->name.c_str(),
                            .color      = {},
                        };

                        fp_vkCmdBeginDebugUtilsLabelEXT(vk_command_buffer, &label);
                    } break;
                    case command_type::pop_user_marker_command:
                        fp_vkCmdEndDebugUtilsLabelEXT(vk_command_buffer);
                        break;
#endif
                    case command_type::copy_buffer_to_buffer_command:
                        break;
                    case command_type::copy_buffer_to_image_command:
                        break;
                    case command_type::copy_image_to_buffer_command:
                        break;
                    case command_type::buffer_memory_barrier_command:
                        break;
                    case command_type::image_memory_barrier_command:
                        break;
                    case command_type::bind_compute_pipeline_object_command:
                        break;
                    case command_type::bind_descriptor_set_command:
                        break;
                    case command_type::push_constant_command:
                        break;
                    case command_type::dispatch_command:
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

                        vkCmdBeginRenderPass(vk_command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
                    } break;
                    case command_type::end_render_pass_command:
                        vkCmdEndRenderPass(vk_command_buffer);
                        break;
                    case command_type::set_viewport_command:
                        break;
                    case command_type::set_scissor_command:
                        break;
                    case command_type::bind_graphics_pipeline_object_command: {
                        auto *command{ reinterpret_cast<recorded_command<command_type::bind_graphics_pipeline_object_command> *>(command_memory) };

                        auto const *pipeline{ resource_cast<vulkan_graphics_pipeline_object const>(command->pipeline_object) };

                        vkCmdBindPipeline(vk_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->get_pipeline_handle());
                    } break;
                    case command_type::bind_vertex_buffer_command: {
                        auto *command{ reinterpret_cast<recorded_command<command_type::bind_vertex_buffer_command> *>(command_memory) };

                        VkBuffer const buffer_handle{ resource_cast<vulkan_buffer const>(command->vertex_buffer)->get_handle() };

                        vkCmdBindVertexBuffers(vk_command_buffer, 0, 1, &buffer_handle, &command->offset);
                    } break;
                    case command_type::bind_index_buffer_command: {
                        auto *command{ reinterpret_cast<recorded_command<command_type::bind_index_buffer_command> *>(command_memory) };

                        VkBuffer const buffer_handle{ resource_cast<vulkan_buffer const>(command->index_buffer)->get_handle() };

                        vkCmdBindIndexBuffer(vk_command_buffer, buffer_handle, command->offset, get_index_type(command->index_type));
                    } break;
                    case command_type::draw_indexed_command: {
                        auto *command{ reinterpret_cast<recorded_command<command_type::draw_indexed_command> *>(command_memory) };

                        vkCmdDrawIndexed(vk_command_buffer, command->index_count, 1, 0, 0, 0);
                    } break;
                    case command_type::present_command: {
                        auto *command{ reinterpret_cast<recorded_command<command_type::present_command> *>(command_memory) };

                        EMBER_VULKAN_VERIFY_RESULT(vkEndCommandBuffer(vk_command_buffer), "Failed to end recording.");

                        //Hit the present command so submit our current buffer
                        array<VkSemaphore> wait_semaphores{};
                        array<VkPipelineStageFlags> wait_stages{};
                        std::size_t const wait_semaphore_count{ submit_info.wait_semaphores.size() };
                        wait_semaphores.resize(wait_semaphore_count);
                        wait_stages.resize(wait_semaphore_count);

                        for(std::size_t i{ 0 }; i < wait_semaphore_count; ++i) {
                            wait_semaphores[i] = resource_cast<vulkan_semaphore const>(submit_info.wait_semaphores[i].first)->get_handle();
                            wait_stages[i]     = convert_stage(submit_info.wait_semaphores[i].second);
                        }

                        array<VkSemaphore> signal_semaphores{};
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
                            .commandBufferCount   = 1,
                            .pCommandBuffers      = &vk_command_buffer,
                            .signalSemaphoreCount = static_cast<std::uint32_t>(signal_semaphore_count),
                            .pSignalSemaphores    = signal_semaphores.data(),
                        };

                        VkFence const fence_handle{ signal_fence != nullptr ? resource_cast<vulkan_fence const>(signal_fence)->get_handle() : VK_NULL_HANDLE };

                        EMBER_VULKAN_VERIFY_RESULT(vkQueueSubmit(handle, 1, &graphics_submit_info, fence_handle), "Failed to submit VkCommandBuffer.");

                        //Add the present submission
                        VkSwapchainKHR const swapchain_handle{ resource_cast<vulkan_swapchain const>(command->swapchain)->get_handle() };
                        auto const image_index{ static_cast<std::uint32_t>(command->image_index) };

                        VkPresentInfoKHR const present_submit_info{
                            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                            .pNext              = nullptr,
                            .waitSemaphoreCount = 0,
                            .pWaitSemaphores    = nullptr,
                            .swapchainCount     = 1,
                            .pSwapchains        = &swapchain_handle,
                            .pImageIndices      = &image_index,
                        };

                        EMBER_VULKAN_VERIFY_RESULT(vkQueuePresentKHR(handle, &present_submit_info), "Error submitting swapchain.");
                    } break;
                    default:
                        EMBER_CHECK(false);
                        break;
                }
            }
        }

        vkFreeCommandBuffers(logical_device, command_pool, 1, &vk_command_buffer);

        TracyVkDestroy(ctx);
    }
}