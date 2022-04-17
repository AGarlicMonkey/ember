#pragma once

#include "device_memory_allocator.hpp"
#include "ember/graphics/device.hpp"
#include "types.hpp"
#include "vulkan_resource_factory.hpp"
#include "vulkan_shader_cache.hpp"

#include <cinttypes>
#include <ember/containers/array.hpp>
#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_device : public device {
        //TYPES
    private:
        struct queue_data {
            std::uint32_t index{ -1u };
            VkQueue queue{ VK_NULL_HANDLE };
            VkCommandPool command_pool{ VK_NULL_HANDLE };
        };

        //VARIABLES
    private:
        VkInstance instance{ VK_NULL_HANDLE };
        VkPhysicalDevice physical_device{ VK_NULL_HANDLE };
        VkDevice logical_device{ VK_NULL_HANDLE };

        queue_family_indices family_indices{};

        memory::unique_ptr<device_memory_allocator> memory_allocator{ nullptr };

        memory::unique_ptr<vulkan_resource_factory> factory{ nullptr };
        memory::unique_ptr<vulkan_shader_cache> cache{ nullptr };

        queue_data graphics_queue_data{};
        queue_data compute_queue_data{};
        queue_data transfer_queue_data{};

        //FUNCTIONS
    public:
        vulkan_device() = delete;
        vulkan_device(VkInstance instance, VkPhysicalDevice physical_device, VkDevice logical_device, queue_family_indices family_indices);

        vulkan_device(vulkan_device const &other) = delete;
        inline vulkan_device(vulkan_device &&other) noexcept;

        vulkan_device &operator=(vulkan_device const &other) = delete;
        inline vulkan_device &operator=(vulkan_device &&other) noexcept;

        ~vulkan_device();

        resource_factory const *get_factory() const override;
        shader_cache *get_shader_cache() override;

        memory::unique_ptr<swapchain> create_swapchain(swapchain::descriptor descriptor, platform::window const &window) const override;

        ~vulkan_device() {
            vkDestroyDevice(logical_device, &global_allocator);
        }

        static queue_family_indices get_physical_device_queue_family_indices(VkPhysicalDevice device);
        static std::int32_t score_physical_device(VkPhysicalDevice physical_device, containers::array<char const *> const &required_extensions);
    };
}

#include "vulkan_device.inl"