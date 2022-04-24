#pragma once

#include "ember/graphics/submission_types.hpp"
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

namespace ember::graphics {
    class command_buffer;
    class fence;
    class semaphore;
    class swapchain;
}

namespace ember::graphics {
    class vulkan_queue {
        //TYPES
    private:
        struct pending_buffer_info {
            containers::array<VkCommandBuffer> buffers{};
            VkFence fence{};
        };

        struct queue {
            VkQueue handle{ VK_NULL_HANDLE };
            VkCommandPool command_pool{ VK_NULL_HANDLE };//TODO: It would be nice to have a pool per frame and just reset at the beginning but the current abstraction does not allow for that.

            containers::array<VkCommandBuffer> pooled_buffers{};
            containers::array<VkFence> pooled_fences{};
            containers::array<pending_buffer_info> pending_buffers{};

#if EMBER_CORE_ENABLE_PROFILING
            TracyVkCtx profiling_context{ nullptr };
            containers::stack<tracy::VkCtxScope> scoped_events{};
            containers::map<std::string, tracy::SourceLocationData> source_datas{};//Bit hack but because of how we do command buffers we need to make our own source locations
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

        vulkan_queue(vulkan_queue const &other) = delete;
        inline vulkan_queue(vulkan_queue &&other) noexcept;

        vulkan_queue &operator=(vulkan_queue const &other) = delete;
        inline vulkan_queue &operator                      =(vulkan_queue &&other) noexcept;

        ~vulkan_queue();

        void submit(graphics_submit_info const &submit_info, fence const *const signal_fence);
        void submit(compute_submit_info const &submit_info, fence const *const signal_fence);
        void submit(transfer_submit_info const &submit_info, fence const *const signal_fence);

        void present(swapchain const *const swapchain, std::size_t const image_index, semaphore const *const wait_semaphore);

    private:
        static void record_commands(queue &queue, VkCommandBuffer vk_buffer, command_buffer const &command_buffer);

        queue create_queue(std::uint32_t const family_index);
        void destroy_queue(queue &queue);

        void reset_available_buffers(queue &queue);
        VkCommandBuffer alloc_buffer(queue &queue);
    };
}

#include "vulkan_queue.inl"