#pragma once

#include "ember/graphics/submission_types.hpp"
#include "types.hpp"

#if EMBER_CORE_ENABLE_PROFILING
    #include <TracyVulkan.hpp>
#endif
#include <vulkan/vulkan.h>

namespace ember::graphics {
    class command_buffer;
    class fence;
}

namespace ember::graphics {
    class vulkan_queue {
        //TYPES
    private:
        struct queue {
            VkQueue handle{ VK_NULL_HANDLE };
            VkCommandPool command_pool{ VK_NULL_HANDLE };//TODO: Pool per frame

#if EMBER_CORE_ENABLE_PROFILING
            VkCommandPool profiling_pool{ VK_NULL_HANDLE };
            VkCommandBuffer profiling_command_buffer{ VK_NULL_HANDLE };
            TracyVkCtx profiling_context{ nullptr };
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
        inline vulkan_queue &operator=(vulkan_queue &&other) noexcept;

        ~vulkan_queue();

        void submit(graphics_submit_info const &submit_info, fence const *const signal_fence);
        void submit(compute_submit_info const &submit_info, fence const *const signal_fence);
        void submit(transfer_submit_info const &submit_info, fence const *const signal_fence);

    private:
        void record_and_submit_commands(VkCommandBuffer vk_buffer, command_buffer const &command_buffer);

        queue create_queue(std::uint32_t const family_index);
        void destroy_queue(queue &queue);
    };
}

#include "vulkan_queue.inl"