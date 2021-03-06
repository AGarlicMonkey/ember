#pragma once

#include "device_memory_allocator.hpp"
#include "ember/graphics/device.hpp"
#include "types.hpp"
#include "vulkan_queue.hpp"
#include "vulkan_resource_factory.hpp"
#include "vulkan_shader_cache.hpp"

#include <cinttypes>
#include <ember/containers/array.hpp>
#include <vulkan/vulkan.h>

namespace ember::inline graphics {
    class vulkan_device : public device {
        //VARIABLES
    private:
        VkInstance instance{ VK_NULL_HANDLE };
        VkPhysicalDevice physical_device{ VK_NULL_HANDLE };
        VkDevice logical_device{ VK_NULL_HANDLE };

        queue_family_indices family_indices{};

        limits device_limits{};

        unique_ptr<device_memory_allocator> memory_allocator{ nullptr };

        unique_ptr<vulkan_resource_factory> factory{ nullptr };
        unique_ptr<vulkan_shader_cache> cache{ nullptr };
        vulkan_queue queue;

        //FUNCTIONS
    public:
        vulkan_device() = delete;
        vulkan_device(VkInstance instance, VkPhysicalDevice physical_device, VkDevice logical_device, queue_family_indices family_indices, limits device_limits);

        vulkan_device(vulkan_device const &other)     = delete;
        vulkan_device(vulkan_device &&other) noexcept = delete;

        vulkan_device &operator=(vulkan_device const &other) = delete;
        vulkan_device &operator=(vulkan_device &&other) noexcept = delete;

        ~vulkan_device();

        limits const &get_limits() const override;

        resource_factory const *get_factory() const override;
        shader_cache *get_shader_cache() override;

        unique_ptr<swapchain> create_swapchain(swapchain::descriptor descriptor, window const &window) const override;

        void submit_to_graphics_queue(graphics_submit_info const &submit_info, fence const *const signal_fence) override;
        void submit_to_compute_queue(compute_submit_info const &submit_info, fence const *const signal_fence) override;
        void submit_to_transfer_queue(transfer_submit_info const &submit_info, fence const *const signal_fence) override;

        swapchain::result present_swapchain(swapchain const *const swapchain, std::size_t const image_index, semaphore const *const wait_semaphore) override;

        void wait_until_idle() const override;

        static queue_family_indices get_physical_device_queue_family_indices(VkPhysicalDevice device);
        static std::int32_t score_physical_device(VkPhysicalDevice physical_device, array<char const *> const &required_extensions);
    };
}