#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_graphics_pipeline_object::vulkan_graphics_pipeline_object(descriptor desc, VkDevice device, VkPipeline pipeline_handle, VkPipelineLayout pipeline_layout_handle)
        : desc{ desc }
        , device{ device }
        , pipeline_handle{ pipeline_handle }
        , pipeline_layout_handle{ pipeline_layout_handle } {
    }

    vulkan_graphics_pipeline_object::vulkan_graphics_pipeline_object(vulkan_graphics_pipeline_object &&other) noexcept = default;

    vulkan_graphics_pipeline_object &vulkan_graphics_pipeline_object::operator=(vulkan_graphics_pipeline_object &&other) noexcept = default;

    vulkan_graphics_pipeline_object::~vulkan_graphics_pipeline_object() {
        vkDestroyPipeline(device, pipeline_handle, &global_host_allocation_callbacks);
        vkDestroyPipelineLayout(device, pipeline_layout_handle, &global_host_allocation_callbacks);
    }

    VkPipeline vulkan_graphics_pipeline_object::get_pipeline_handle() const {
        return pipeline_handle;
    }

    VkPipelineLayout vulkan_graphics_pipeline_object::get_layout_handle() const {
        return pipeline_layout_handle;
    }
}