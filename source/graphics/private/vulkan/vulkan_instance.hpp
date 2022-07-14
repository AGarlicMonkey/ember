#pragma once

#include "ember/graphics/instance.hpp"

#include <ember/memory/unique_ptr.hpp>
#include <ember/containers/array.hpp>
#include <vulkan/vulkan.h>

namespace ember::inline graphics {
    class vulkan_device;
}

namespace ember::inline graphics {
    class vulkan_instance : public instance {
        //VARIABLES
    private:
        VkInstance instance{ VK_NULL_HANDLE };
        VkDebugUtilsMessengerEXT debug_messenger{ VK_NULL_HANDLE };

        unique_ptr<vulkan_device> device{};

        //FUNCTIONS
    public:
        vulkan_instance() = delete;
        vulkan_instance(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, unique_ptr<vulkan_device> device);

        vulkan_instance(vulkan_instance const &other) = delete;
        vulkan_instance(vulkan_instance &&other) noexcept = delete;

        vulkan_instance &operator=(vulkan_instance const &other) = delete;
        vulkan_instance &operator=(vulkan_instance &&other) noexcept = delete;

        ~vulkan_instance();

        graphics::device *get_device() const noexcept override;
    };
}

namespace ember::inline graphics {
    unique_ptr<vulkan_instance> create_vulkan_instance();
}