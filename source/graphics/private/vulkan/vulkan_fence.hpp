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
        inline vulkan_fence(vulkan_fence &&other) noexcept;

        vulkan_fence &operator=(vulkan_fence const &other) = delete;
        inline vulkan_fence &operator=(vulkan_fence &&other) noexcept;

        inline ~vulkan_fence() override;

        descriptor const &get_descriptor() const override;

        void wait() override;
        void reset() override;

        inline VkFence get_handle() const;
    };
}

#include "vulkan_fence.inl"