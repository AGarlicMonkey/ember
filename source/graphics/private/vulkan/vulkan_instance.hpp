#pragma once

#include "ember/graphics/instance.hpp"

#include <ember/memory/unique_ptr.hpp>
#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_instance : public instance {
        //VARIABLES
    private:
        VkInstance instance{ VK_NULL_HANDLE };
        VkDebugUtilsMessengerEXT debug_messenger{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_instance() = delete;
        inline vulkan_instance(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger);

        vulkan_instance(vulkan_instance const &other) = delete;
        inline vulkan_instance(vulkan_instance &&other) noexcept;

        vulkan_instance &operator=(vulkan_instance const &other) = delete;
        inline vulkan_instance &operator=(vulkan_instance &&other) noexcept;

        ~vulkan_instance();
    };
}

#include "vulkan_instance.inl"

namespace ember::graphics {
    memory::unique_ptr<vulkan_instance> create_vulkan_instance();
}