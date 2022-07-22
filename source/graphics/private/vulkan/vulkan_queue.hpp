#pragma once

#include "ember/graphics/submission_types.hpp"
#include "ember/graphics/swapchain.hpp"
#include "ember/graphics/transfer_command_buffer.hpp"
#include "types.hpp"

#include <ember/containers/array.hpp>
#include <ember/containers/map.hpp>
#include <ember/containers/stack.hpp>
#include <vulkan/vulkan.h>
#if EMBER_CORE_ENABLE_PROFILING
    //NOTE: Needs to be after vulkan headers
    #include <TracyVulkan.hpp>

    #if defined TRACY_HAS_CALLSTACK && defined TRACY_CALLSTACK
        #error "Current config is not supported"
    #endif
#endif

namespace ember::inline graphics {
    class command_buffer;
    class fence;
    class semaphore;
    class swapchain;
}

namespace ember::inline graphics {
    class vulkan_queue {
        //TYPES
    private:
        struct pending_buffer_info {
            array<VkCommandBuffer> buffers{};
            VkFence fence{};
        };

#if EMBER_CORE_ENABLE_PROFILING
        struct source_data {
            std::string name;
            tracy::SourceLocationData source_location;
        };
#endif

        struct queue {
            VkQueue handle{ VK_NULL_HANDLE };
            VkCommandPool command_pool{ VK_NULL_HANDLE };

            array<VkCommandBuffer> pooled_buffers{};
            array<VkFence> pooled_fences{};
            array<pending_buffer_info> pending_buffers{};

#if EMBER_CORE_ENABLE_PROFILING
            TracyVkCtx profiling_context{ nullptr };
            stack<tracy::VkCtxScope> scoped_events{};
            map<std::string, source_data> source_datas{};//Bit hack but because of how we do command buffers we need to make our own source locations and these need to be stored somewhere
#endif
        };

        //VARIABLES
    private:
        VkPhysicalDevice physical_device{ VK_NULL_HANDLE };
        VkDevice logical_device{ VK_NULL_HANDLE };

        queue_family_indices family_indices{};

        queue graphics_queue{};
        queue compute_queue{};
        queue transfer_queue{};

        //FUNCTIONS
    public:
        vulkan_queue() = delete;
        vulkan_queue(VkPhysicalDevice physical_device, VkDevice logical_device, queue_family_indices family_indices);

        vulkan_queue(vulkan_queue const &other)     = delete;
        vulkan_queue(vulkan_queue &&other) noexcept = delete;

        vulkan_queue &operator=(vulkan_queue const &other)     = delete;
        vulkan_queue &operator=(vulkan_queue &&other) noexcept = delete;

        ~vulkan_queue();

        void submit(graphics_submit_info const &submit_info, fence const *const signal_fence);
        void submit(compute_submit_info const &submit_info, fence const *const signal_fence);
        void submit(transfer_submit_info const &submit_info, fence const *const signal_fence);

        swapchain::result present(swapchain const *const swapchain, std::size_t const image_index, semaphore const *const wait_semaphore);

        void release_resources();

    private:
        void record_commands(queue &queue, VkPipelineBindPoint const bind_point, VkCommandBuffer vk_buffer, command_buffer const &command_buffer);

        queue create_queue(std::uint32_t const family_index, std::string_view name);
        template<typename submit_info_t>
        void submit_to_queue(queue &queue, VkPipelineBindPoint const bind_point, submit_info_t const &submit_info, fence const *const signal_fence);
        void destroy_queue(queue &queue);

        std::uint32_t get_queue_family_index(queue_type const queue_type);

        void reset_available_buffers(queue &queue);
        VkCommandBuffer alloc_buffer(queue &queue);
    };
}