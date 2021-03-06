#pragma once

#include "ember/graphics/shader.hpp"

#include <vulkan/vulkan.h>

namespace ember::inline graphics {
    class vulkan_shader : public shader {
        //VARIABLES
    private:
        VkDevice device{ VK_NULL_HANDLE };
        VkShaderModule module{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_shader() = delete;
        inline vulkan_shader(VkDevice device, VkShaderModule module);

        vulkan_shader(vulkan_shader const &other)     = delete;
        vulkan_shader(vulkan_shader &&other) noexcept = delete;

        vulkan_shader &operator=(vulkan_shader const &other) = delete;
        vulkan_shader &operator=(vulkan_shader &&other) noexcept = delete;

        inline ~vulkan_shader();

        inline VkShaderModule get_module() const;

        static VkShaderStageFlags convert_stage(stage const stage);
    };
}

#include "vulkan_shader.inl"