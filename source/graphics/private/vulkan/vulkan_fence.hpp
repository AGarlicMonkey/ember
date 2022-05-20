#pragma once

#include "ember/graphics/fence.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_fence : public fence {
        //VARIABLES
    private:
        descriptor desc{};

        VkDevice device{ VK_NULL_HANDLE };
        VkFence handle{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_fence() = delete;
        inline vulkan_fence(descriptor desc, VkDevice device, VkFence handle);

        vulkan_fence(vulkan_fence const &other) = delete;
        vulkan_fence(vulkan_fence &&other) noexcept = delete;

        vulkan_fence &operator=(vulkan_fence const &other) = delete;
        vulkan_fence &operator=(vulkan_fence &&other) noexcept = delete;

        inline ~vulkan_fence() override;

        descriptor const &get_descriptor() const override;

        void wait() override;
        bool poll() override;
        
        void reset() override;

        inline VkFence get_handle() const;
    };
}

#include "vulkan_fence.inl"