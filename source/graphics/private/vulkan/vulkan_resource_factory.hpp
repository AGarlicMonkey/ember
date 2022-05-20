#pragma once

#include "device_memory_allocator.hpp"
#include "ember/graphics/resource_factory.hpp"
#include "types.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_device;
}

namespace ember::graphics {
    class vulkan_resource_factory : public resource_factory {
        //VARIABLES
    private:
        VkDevice device{ VK_NULL_HANDLE };
        VkPhysicalDeviceProperties physical_device_poperties{};
        queue_family_indices family_indices{};
        device_memory_allocator *memory_allocator{ nullptr };

        //FUNCTIONS
    public:
        vulkan_resource_factory() = delete;
        vulkan_resource_factory(VkDevice device, VkPhysicalDevice physical_device, queue_family_indices family_indices, device_memory_allocator *memory_allocator);

        vulkan_resource_factory(vulkan_resource_factory const &other) = delete;
        vulkan_resource_factory(vulkan_resource_factory &&other) noexcept = delete;

        vulkan_resource_factory &operator=(vulkan_resource_factory const &other) = delete;
        vulkan_resource_factory &operator=(vulkan_resource_factory &&other) noexcept = delete;

        inline ~vulkan_resource_factory() override;

        memory::unique_ptr<buffer> create_buffer(buffer::descriptor descriptor, std::string_view name) const override;
        memory::unique_ptr<image> create_image(image::descriptor descriptor, std::string_view name) const override;
        memory::unique_ptr<image_view> create_image_view(image_view::descriptor descriptor, image const &image, std::string_view name) const override;
        memory::unique_ptr<sampler> create_sampler(sampler::descriptor descriptor, std::string_view name) const override;

        memory::unique_ptr<graphics_pipeline_object> create_graphics_pipeline_object(graphics_pipeline_object::descriptor descriptor, std::string_view name) const override;

        memory::unique_ptr<descriptor_set_layout> create_descriptor_set_layout(descriptor_set_layout::descriptor descriptor, std::string_view name) const override;

        memory::unique_ptr<render_pass> create_render_pass(render_pass::descriptor descriptor, std::string_view name) const override;
        memory::unique_ptr<framebuffer> create_framebuffer(framebuffer::descriptor descriptor, std::string_view name) const override;

        memory::unique_ptr<fence> create_fence(fence::descriptor descriptor, std::string_view name) const override;
        memory::unique_ptr<semaphore> create_semaphore(std::string_view name) const override;
    };
}

#include "vulkan_resource_factory.inl"