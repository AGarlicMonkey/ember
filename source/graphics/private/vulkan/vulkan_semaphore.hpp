#pragma once

#include "ember/graphics/semaphore.hpp"

#include <vulkan/vulkan.h>

namespace ember::inline graphics {
    class vulkan_semaphore : public semaphore {
        //VARIABLES
    private:
        VkDevice device{ VK_NULL_HANDLE };
        VkSemaphore handle{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_semaphore() = delete;
        inline vulkan_semaphore(VkDevice device, VkSemaphore handle);

        vulkan_semaphore(vulkan_semaphore const &other) = delete;
        vulkan_semaphore(vulkan_semaphore &&other) noexcept = delete;

        vulkan_semaphore &operator=(vulkan_semaphore const &other) = delete;
        vulkan_semaphore &operator=(vulkan_semaphore &&other) noexcept = delete;

        inline ~vulkan_semaphore() override;

        inline VkSemaphore get_handle() const;
    };
}

#include "vulkan_semaphore.inl"