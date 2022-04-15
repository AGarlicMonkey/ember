#include "vulkan_swapchain.hpp"

#include "allocation_callbacks.hpp"
#include "log.hpp"
#include "vulkan_image.hpp"
#include "vulkan_semaphore.hpp"

using namespace ember::containers;
using namespace ember::maths;
using namespace ember::memory;

namespace ember::graphics {
    vulkan_swapchain::vulkan_swapchain(VkInstance instance, VkDevice device, VkSurfaceKHR surface, VkSwapchainKHR swapchain, VkFormat swapchain_image_format, VkExtent2D swapchain_extent, array<unique_ptr<vulkan_image>> images)
        : instance{ instance }
        , device{ device }
        , swapchain{ swapchain }
        , surface{ surface }
        , swapchain_image_format{ swapchain_image_format }
        , swapchain_extent{ swapchain_extent }
        , global_allocator{ get_allocation_callbacks() }
        , images{ std::move(images) } {
    }

    std::pair<std::uint32_t, vulkan_swapchain::result> vulkan_swapchain::aquire_next_image(semaphore const *available_semaphore) {
        std::uint32_t out_image_index{ 0 };
        VkSemaphore vk_semaphore{ available_semaphore != nullptr ? static_cast<vulkan_semaphore const *>(available_semaphore)->get_semaphore() : VK_NULL_HANDLE };
        VkResult const result{ vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, vk_semaphore, VK_NULL_HANDLE, &out_image_index) };

        return { out_image_index, convert_result(result) };
    }

    image::format vulkan_swapchain::get_image_format() const {
        return vulkan_image::convert_format(swapchain_image_format);
    }

    vec2u vulkan_swapchain::get_size() const {
        return { swapchain_extent.width, swapchain_extent.height };
    }

    array<image *> vulkan_swapchain::get_images() const {
        array<image *> out_images{};
        //out_images.reserve(images.size()); //TODO

        for(auto const &image : images) {
            out_images.push_back(image.get());
        }

        return out_images;
    }

    vulkan_swapchain::result vulkan_swapchain::convert_result(VkResult const result) {
        switch(result) {
            case VK_SUCCESS:
                return result::success;
            case VK_SUBOPTIMAL_KHR:
                return result::success_swapchain_suboptimal;
            case VK_ERROR_OUT_OF_DATE_KHR:
                return result::error_swapchain_out_of_date;
            default:
                EMBER_CHECK_MSG(false, "Unknown result type");
                return result::error;
        }
    }

    VkSurfaceFormatKHR vulkan_swapchain::choose_surface_format(array<VkSurfaceFormatKHR> const &available_formats) {
        for(auto const &format : available_formats) {
            if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return format;
            }
        }

        //Fall back to the first one if we can't find a surface format we want
        EMBER_LOG(EmberGraphicsVulkan, log_level::warn, "Could not find desired format for swapchain. Using first available format from the surface");
        return available_formats[0];
    }

    VkPresentModeKHR vulkan_swapchain::choose_present_mode(array<VkPresentModeKHR> const &available_present_modes) {
        for(auto const &present_mode : available_present_modes) {
            if(present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return present_mode;
            }
        }

        EMBER_LOG(EmberGraphicsVulkan, log_level::warn, "Could not find VK_PRESENT_MODE_MAILBOX_KHR in available present modes. Defaulting to VK_PRESENT_MODE_FIFO_KHR");
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D vulkan_swapchain::choose_extent(VkSurfaceCapabilitiesKHR const &capabilities, VkExtent2D const &window_extent) {
        if(capabilities.currentExtent.width != UINT32_MAX) {//If width / height are max then the window manager is allowing us to differ in size
            return capabilities.currentExtent;
        } else {
            return VkExtent2D{
                .width  = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, window_extent.width)),
                .height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, window_extent.height)),
            };
        }
    }
}