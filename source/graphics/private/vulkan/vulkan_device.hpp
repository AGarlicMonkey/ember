#pragma once

#include "ember/graphics/device.hpp"
#include "allocation_callbacks.hpp"

#include <ember/containers/array.hpp>
#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_device : public device {
        //TYPES
    public:
        struct queue_family_indices {
            std::uint32_t graphics_family{ -1u };//Will act as both the graphics and present queue
            std::uint32_t compute_family{ -1u };
            std::uint32_t transfer_family{ -1u };
        };

        //VARIABLES
    private:
        VkPhysicalDevice physical_device{ VK_NULL_HANDLE };
        VkDevice logical_device{ VK_NULL_HANDLE };

        VkAllocationCallbacks global_allocator{};

        //FUNCTIONS
    public:
        vulkan_device() = delete;
        vulkan_device(VkPhysicalDevice physical_device, VkDevice logical_device)
            : physical_device{ physical_device }
            , logical_device{ logical_device }
            , global_allocator{ get_allocations_callbacks() } {
        }

        //TODO: proper ctors

        ~vulkan_device() {
            vkDestroyDevice(logical_device, &global_allocator);
        }

        static queue_family_indices get_physical_device_queue_family_indices(VkPhysicalDevice device);
        static std::int32_t score_physical_device(VkPhysicalDevice physical_device, containers::array<char const *> const &required_extensions);
    };
}