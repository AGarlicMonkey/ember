#pragma once

#include "ember/graphics/graphics_queue.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_graphics_queue : public graphics_queue {
        //VARIABLES
    private:
        VkPhysicalDevice physical_device{ VK_NULL_HANDLE };
        VkDevice logical_device{ VK_NULL_HANDLE };
        VkQueue handle{ VK_NULL_HANDLE };
        VkCommandPool command_pool{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_graphics_queue() = delete;
        vulkan_graphics_queue(VkPhysicalDevice physical_device, VkDevice logical_device, VkQueue handle, VkCommandPool command_pool);

        vulkan_graphics_queue(vulkan_graphics_queue const &other) = delete;
        inline vulkan_graphics_queue(vulkan_graphics_queue &&other) noexcept;

        vulkan_graphics_queue &operator=(vulkan_graphics_queue const &other) = delete;
        inline vulkan_graphics_queue &operator=(vulkan_graphics_queue &&other) noexcept;

        inline ~vulkan_graphics_queue() override;

        void submit(graphics_submit_info const &submit_info, fence const *const signal_fence) override;
    };
}

#include "vulkan_graphics_queue.inl"