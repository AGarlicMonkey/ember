#pragma once

#include "ember/graphics/framebuffer.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_framebuffer : public framebuffer {
        //VARIABLES
    private:
        descriptor desc{};

        VkDevice device{ VK_NULL_HANDLE };
        VkFramebuffer handle{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_framebuffer() = delete;
        inline vulkan_framebuffer(descriptor desc, VkDevice device, VkFramebuffer handle);

        vulkan_framebuffer(vulkan_framebuffer const &other)            = delete;
        vulkan_framebuffer(vulkan_framebuffer &&other) noexcept = delete;

        vulkan_framebuffer &operator=(vulkan_framebuffer const &other) = delete;
        vulkan_framebuffer &operator=(vulkan_framebuffer &&other) noexcept = delete;

        inline ~vulkan_framebuffer() override;

        descriptor const &get_descriptor() const override;

        inline VkFramebuffer get_handle() const;
    };
}

#include "vulkan_framebuffer.inl"