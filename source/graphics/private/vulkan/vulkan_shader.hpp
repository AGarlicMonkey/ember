#pragma once

#include "ember/graphics/shader.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_shader : public shader {
        //VARIABLES
    private:
        VkDevice device{ VK_NULL_HANDLE };
        VkShaderModule module{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_shader() = delete;
        inline vulkan_shader(VkDevice device, VkShaderModule module);

        vulkan_shader(vulkan_shader const &other) = delete;
        inline vulkan_shader(vulkan_shader &&other) noexcept;

        vulkan_shader &operator=(vulkan_shader const &other) = delete;
        inline vulkan_shader &operator=(vulkan_shader &&other) noexcept;

        inline ~vulkan_shader();
    };
}

#include "vulkan_shader.inl"