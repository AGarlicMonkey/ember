#pragma once

#include "device_memory_allocator.hpp"
#include "ember/graphics/resource_factory.hpp"
#include "types.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_resource_factory : public resource_factory {
        //VARIABLES
    private:
        VkDevice device{ VK_NULL_HANDLE };
        queue_family_indices family_indices{};
        device_memory_allocator *memory_allocator{ nullptr };

        //FUNCTIONS
    public:
        vulkan_resource_factory() = delete;
        vulkan_resource_factory(VkInstance instance, VkDevice device, queue_family_indices family_indices, device_memory_allocator *memory_allocator);

        vulkan_resource_factory(vulkan_resource_factory const &other) = delete;
        inline vulkan_resource_factory(vulkan_resource_factory &&other) noexcept;

        vulkan_resource_factory &operator=(vulkan_resource_factory const &other) = delete;
        inline vulkan_resource_factory &operator=(vulkan_resource_factory &&other) noexcept;

        inline ~vulkan_resource_factory() override;

        memory::unique_ptr<buffer> create_buffer(buffer::descriptor const &descriptor, std::string_view name) const override;
    };
}

#include "vulkan_resource_factory.inl"