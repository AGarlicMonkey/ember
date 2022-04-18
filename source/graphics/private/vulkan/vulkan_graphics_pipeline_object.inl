#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_graphics_pipeline_object::vulkan_graphics_pipeline_object(VkDevice device, VkPipeline pipeline, VkPipelineLayout pipeline_layout)
        : device{ device }
        , pipeline{ pipeline }
        , pipeline_layout{ pipeline_layout } {
    }

    vulkan_graphics_pipeline_object::vulkan_graphics_pipeline_object(vulkan_graphics_pipeline_object &&other) noexcept = default;

    vulkan_graphics_pipeline_object &vulkan_graphics_pipeline_object::operator=(vulkan_graphics_pipeline_object &&other) noexcept = default;

    vulkan_graphics_pipeline_object::~vulkan_graphics_pipeline_object() {
        vkDestroyPipeline(device, pipeline, &global_host_allocation_callbacks);
        vkDestroyPipelineLayout(device, pipeline_layout, &global_host_allocation_callbacks);
    }

    VkPipeline vulkan_graphics_pipeline_object::get_pipeline() const {
        return pipeline;
    }

    VkPipelineLayout vulkan_graphics_pipeline_object::get_layout() const {
        return pipeline_layout;
    }
}