#pragma once

#include "ember/graphics/graphics_pipeline_object.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_graphics_pipeline_object : public graphics_pipeline_object {
        //VARIABLES
    private:
        VkDevice device{ VK_NULL_HANDLE };
        VkPipeline pipeline{ VK_NULL_HANDLE };
        VkPipelineLayout pipeline_layout{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_graphics_pipeline_object() = delete;
        inline vulkan_graphics_pipeline_object(VkDevice device, VkPipeline pipeline, VkPipelineLayout pipeline_layout);

        vulkan_graphics_pipeline_object(vulkan_graphics_pipeline_object const &other) = delete;
        inline vulkan_graphics_pipeline_object(vulkan_graphics_pipeline_object &&other) noexcept;

        vulkan_graphics_pipeline_object &operator=(vulkan_graphics_pipeline_object const &other) = delete;
        inline vulkan_graphics_pipeline_object &operator=(vulkan_graphics_pipeline_object &&other) noexcept;

        inline ~vulkan_graphics_pipeline_object() override;

        inline VkPipeline get_pipeline() const;
        inline VkPipelineLayout get_layout() const;

        static VkFormat convert_attribute_format(vertex_attribute_format const format);
    };
}

#include "vulkan_graphics_pipeline_object.inl"