#pragma once

#include "ember/graphics/compute_pipeline_object.hpp"

#include <vulkan/vulkan.h>

namespace ember::inline graphics {
    class vulkan_compute_pipeline_object : public compute_pipeline_object {
        //VARIABLES
    private:
        descriptor desc{};

        VkDevice device{ VK_NULL_HANDLE };
        VkPipeline pipeline_handle{ VK_NULL_HANDLE };
        VkPipelineLayout pipeline_layout_handle{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_compute_pipeline_object() = delete;
        inline vulkan_compute_pipeline_object(descriptor desc, VkDevice device, VkPipeline pipeline_handle, VkPipelineLayout pipeline_layout_handle);

        vulkan_compute_pipeline_object(vulkan_compute_pipeline_object const &other)      = delete;
        vulkan_compute_pipeline_object(vulkan_compute_pipeline_object &&other) noexcept  = delete;

        vulkan_compute_pipeline_object &operator=(vulkan_compute_pipeline_object const &other)     = delete;
        vulkan_compute_pipeline_object &operator=(vulkan_compute_pipeline_object &&other) noexcept = delete;

        inline ~vulkan_compute_pipeline_object() override;

        descriptor const &get_descriptor() const override;

        inline VkPipeline get_pipeline_handle() const;
        inline VkPipelineLayout get_layout_handle() const;
    };
}

#include "vulkan_compute_pipeline_object.inl"