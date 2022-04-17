#pragma once

#include "ember/graphics/instance.hpp"

#include <ember/memory/unique_ptr.hpp>
#include <ember/containers/array.hpp>
#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_device;
}

namespace ember::graphics {
    class vulkan_instance : public instance {
        //VARIABLES
    private:
        VkInstance instance{ VK_NULL_HANDLE };
        VkDebugUtilsMessengerEXT debug_messenger{ VK_NULL_HANDLE };

        memory::unique_ptr<vulkan_device> device{};

        //FUNCTIONS
    public:
        vulkan_instance() = delete;
        vulkan_instance(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, memory::unique_ptr<vulkan_device> device);

        vulkan_instance(vulkan_instance const &other) = delete;
        vulkan_instance(vulkan_instance &&other) noexcept;

        vulkan_instance &operator=(vulkan_instance const &other) = delete;
        vulkan_instance &operator=(vulkan_instance &&other) noexcept;

        ~vulkan_instance();

        graphics::device *get_device() const noexcept override;
    };
}

namespace ember::graphics {
    memory::unique_ptr<vulkan_instance> create_vulkan_instance();
}