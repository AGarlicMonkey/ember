#pragma once

#include "ember/graphics/swapchain.hpp"
#include "vulkan_image.hpp"

#include <ember/containers/array.hpp>
#include <ember/memory/unique_ptr.hpp>
#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_swapchain : public swapchain {
        //VARIABLES
    private:
        VkInstance instance{ VK_NULL_HANDLE };
        VkDevice device{ VK_NULL_HANDLE };
        VkSurfaceKHR surface{ VK_NULL_HANDLE };
        VkSwapchainKHR handle{ VK_NULL_HANDLE };

        VkFormat swapchain_image_format{};
        VkExtent2D swapchain_extent{};

        containers::array<memory::unique_ptr<vulkan_image>> images{};

        //FUNCTIONS
    public:
        vulkan_swapchain() = delete;
        vulkan_swapchain(VkInstance instance, VkDevice device, VkSurfaceKHR surface, VkSwapchainKHR handle, VkFormat swapchain_image_format, VkExtent2D swapchain_extent, containers::array<memory::unique_ptr<vulkan_image>> images);

        vulkan_swapchain(vulkan_swapchain const &other) = delete;
        vulkan_swapchain(vulkan_swapchain &&other) noexcept = delete;

        vulkan_swapchain &operator=(vulkan_swapchain const &other) = delete;
        vulkan_swapchain &operator=(vulkan_swapchain &&other) noexcept = delete;

        inline ~vulkan_swapchain();

        std::pair<std::uint32_t, result> aquire_next_image(semaphore const *available_semaphore) override;

        image::format get_image_format() const override;
        maths::vec2u get_size() const override;

        containers::array<image *> get_images() const override;

        inline VkSwapchainKHR get_handle() const;

        static result convert_result(VkResult const result);

        static VkSurfaceFormatKHR choose_surface_format(containers::array<VkSurfaceFormatKHR> const &available_formats);
        static VkPresentModeKHR choose_present_mode(containers::array<VkPresentModeKHR> const &available_present_modes);
        static VkExtent2D choose_extent(VkSurfaceCapabilitiesKHR const &capabilities, VkExtent2D const &window_extent);
    };
}

#include "vulkan_swapchain.inl"