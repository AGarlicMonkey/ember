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
        //TODO: ctors
        vulkan_shader() = delete;
        inline vulkan_shader(VkDevice device, VkShaderModule module);

        inline ~vulkan_shader();
    };
}

#include "vulkan_shader.inl"