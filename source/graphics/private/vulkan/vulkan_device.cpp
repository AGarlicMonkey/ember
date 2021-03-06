#if EMBER_PLATFORM_WIN32
    #define VK_USE_PLATFORM_WIN32_KHR
#elif EMBER_PLATFORM_MACOS
    #error vulkan not supported on macos
#elif EMBER_PLATFORM_XLIB
    #error vulkan not supported on linux
#endif

// clang-format off
#include <vulkan/vk_sdk_platform.h>
// clang-format on

#include "vulkan_device.hpp"

#include "host_memory_allocator.hpp"
#include "log.hpp"
#include "verification.hpp"
#include "vulkan_image.hpp"
#include "vulkan_swapchain.hpp"

#include <ember/containers/array.hpp>
#include <ember/containers/set.hpp>
#include <ember/memory/unique_ptr.hpp>
#include <ember/platform/window.hpp>

namespace {
    bool check_device_extension_support(VkPhysicalDevice device, ember::array<char const *> const &extensions) {
        uint32_t extension_count{ 0 };
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
        ember::array<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

        for(char const *extensionName : extensions) {
            bool found = false;

            for(auto const &extensionProperties : available_extensions) {
                if(std::strcmp(extensionName, extensionProperties.extensionName) == 0) {
                    found = true;
                    break;
                }
            }

            if(!found) {
                return false;
            }
        }

        return true;
    }
}

namespace ember::inline graphics {
    vulkan_device::vulkan_device(VkInstance instance, VkPhysicalDevice physical_device, VkDevice logical_device, queue_family_indices family_indices, limits device_limits)
        : instance{ instance }
        , physical_device{ physical_device }
        , logical_device{ logical_device }
        , family_indices{ family_indices }
        , device_limits{ device_limits }
        , queue{ physical_device, logical_device, family_indices } {
        memory_allocator = make_unique<device_memory_allocator>(logical_device, physical_device);
        factory          = make_unique<vulkan_resource_factory>(logical_device, physical_device, this->family_indices, memory_allocator.get());
        cache            = make_unique<vulkan_shader_cache>(logical_device);
    }

    vulkan_device::~vulkan_device() {
        queue.release_resources();

        //Call dtor to make sure memory is freed before destroying the device.
        memory_allocator.reset();
        cache.reset();

        vkDestroyDevice(logical_device, &global_host_allocation_callbacks);
    }

    device::limits const &vulkan_device::get_limits() const {
        return device_limits;
    }

    resource_factory const *vulkan_device::get_factory() const {
        return factory.get();
    }

    shader_cache *vulkan_device::get_shader_cache() {
        return cache.get();
    }

    unique_ptr<swapchain> vulkan_device::create_swapchain(swapchain::descriptor descriptor, window const &window) const {
#if EMBER_PLATFORM_WIN32
        VkWin32SurfaceCreateInfoKHR const surface_create_info{
            .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .hinstance = GetModuleHandle(nullptr),
            .hwnd      = reinterpret_cast<HWND>(window.get_native_window()),
        };

        VkSurfaceKHR surface{ VK_NULL_HANDLE };
        EMBER_VULKAN_VERIFY_RESULT(vkCreateWin32SurfaceKHR(instance, &surface_create_info, &global_host_allocation_callbacks, &surface), "Failed to create swapchain. Could not create surface.");
#elif EMBER_PLATFORM_MACOS
    #error vulkan not supported on macos
#elif EMBER_PLATFORM_XLIB
        //TODO
#endif

        VkBool32 present_support{ VK_FALSE };
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, family_indices.graphics, surface, &present_support);
        if(present_support == VK_FALSE) {
            vkDestroySurfaceKHR(instance, surface, &global_host_allocation_callbacks);
            EMBER_THROW(present_not_available_exception{ "Graphics queue or physical device does not have presentation support." });
        }

        VkExtent2D const extent{ descriptor.extent.x, descriptor.extent.y };

        VkSurfaceCapabilitiesKHR capabilities{};
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities);

        std::uint32_t format_count{ 0 };
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
        EMBER_THROW_IF_FAILED(format_count > 0, graphics_exception{ "Surface does not have any formats." });
        array<VkSurfaceFormatKHR> formats(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, formats.data());

        std::uint32_t present_mode_count{ 0 };
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);
        EMBER_THROW_IF_FAILED(format_count > 0, graphics_exception{ "Surface does not have any present modes." });
        array<VkPresentModeKHR> present_modes(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes.data());

        VkSurfaceFormatKHR const surface_format{ vulkan_swapchain::choose_surface_format(formats) };
        VkPresentModeKHR const present_mode{ vulkan_swapchain::choose_present_mode(present_modes) };
        VkExtent2D const swapchain_extent{ vulkan_swapchain::choose_extent(capabilities, extent) };

        std::uint32_t desired_image_count{ descriptor.image_count };
        std::uint32_t const max_image_count{ capabilities.maxImageCount };
        if(desired_image_count > max_image_count) {
            EMBER_LOG(EmberGraphicsVulkan, log_level::warn, "{0} images requirested from the surface but only {1} were available. Creating a swapchain of {1} images.", desired_image_count, max_image_count);
            desired_image_count = max_image_count;
        }

        VkSwapchainCreateInfoKHR const swapchain_create_info{
            .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface               = surface,
            .minImageCount         = desired_image_count,
            .imageFormat           = surface_format.format,
            .imageColorSpace       = surface_format.colorSpace,
            .imageExtent           = swapchain_extent,
            .imageArrayLayers      = 1,
            .imageUsage            = vulkan_image::convert_usage(descriptor.usage_flags),
            .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices   = nullptr,
            .preTransform          = capabilities.currentTransform,
            .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode           = present_mode,
            .clipped               = VK_TRUE,
            .oldSwapchain          = VK_NULL_HANDLE,
        };

        VkSwapchainKHR swapchain{ nullptr };
        EMBER_VULKAN_VERIFY_RESULT(vkCreateSwapchainKHR(logical_device, &swapchain_create_info, &global_host_allocation_callbacks, &swapchain), "Failed to create vk_swapchain. ");

        array<VkImage> vk_images{};
        array<unique_ptr<vulkan_image>> vulkan_images{};

        image::format const image_format{ vulkan_image::convert_format(surface_format.format) };
        vec2u const swapchain_size{ extent.width, extent.height };

        //Descriptor is reconstructed from the image settings in the VkSwapchainCreateInfoKHR
        image::descriptor const image_descriptor{
            .type         = image::type::_2d,
            .usage_flags  = image::usage_mode::colour_attachment,
            .dimensions   = swapchain_size,
            .array_count  = 1,
            .format       = image_format,
            .sharing_mode = sharing_mode::exclusive,
        };
        //TODO: Resource name

        std::uint32_t created_image_count{ 0 };
        vkGetSwapchainImagesKHR(logical_device, swapchain, &created_image_count, nullptr);
        vk_images.resize(created_image_count);
        vkGetSwapchainImagesKHR(logical_device, swapchain, &created_image_count, vk_images.data());

        vulkan_images.resize(vk_images.size());
        for(size_t i{ 0 }; i < vk_images.size(); ++i) {
            vulkan_images[i] = make_unique<vulkan_image>(image_descriptor, logical_device, vk_images[i]);
        }

        return make_unique<vulkan_swapchain>(instance, logical_device, surface, swapchain, surface_format.format, extent, std::move(vulkan_images));
    }

    void vulkan_device::submit_to_graphics_queue(graphics_submit_info const &submit_info, fence const *const signal_fence) {
        queue.submit(submit_info, signal_fence);
    }

    void vulkan_device::submit_to_compute_queue(compute_submit_info const &submit_info, fence const *const signal_fence) {
        queue.submit(submit_info, signal_fence);
    }

    void vulkan_device::submit_to_transfer_queue(transfer_submit_info const &submit_info, fence const *const signal_fence) {
        queue.submit(submit_info, signal_fence);
    }

    swapchain::result vulkan_device::present_swapchain(swapchain const *const swapchain, std::size_t const image_index, semaphore const *const wait_semaphore) {
        return queue.present(swapchain, image_index, wait_semaphore);
    }

    void vulkan_device::wait_until_idle() const {
        vkDeviceWaitIdle(logical_device);
    }

    queue_family_indices vulkan_device::get_physical_device_queue_family_indices(VkPhysicalDevice device) {
        queue_family_indices indices{};
        set<std::uint32_t> graphics_families{};
        set<std::uint32_t> compute_families{};
        set<std::uint32_t> transfer_families{};

        std::uint32_t queue_family_count{ 0 };
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
        array<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

        for(std::uint32_t i{ 0 }; auto const &queue_family : queue_families) {
            if((queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0u) {
                graphics_families.emplace(i);
            }
            if((queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT) != 0u) {
                compute_families.emplace(i);
            }
            if((queue_family.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0u) {
                transfer_families.emplace(i);
            }

            ++i;
        }

        indices.graphics = *graphics_families.begin();

        //Try and make compute and transfer different families if possible
        if(compute_families.size() > 1) {
            compute_families.erase(indices.graphics);
        }
        if(transfer_families.size() > 1) {
            transfer_families.erase(indices.graphics);
        }

        indices.compute = *compute_families.begin();

        if(transfer_families.size() > 1) {
            transfer_families.erase(indices.compute);
        }

        indices.transfer = *transfer_families.begin();

        return indices;
    }

    std::int32_t vulkan_device::score_physical_device(VkPhysicalDevice physical_device, array<char const *> const &required_extensions) {
        std::int32_t score{ 0 };

        VkPhysicalDeviceFeatures deviceFeatures{};
        vkGetPhysicalDeviceFeatures(physical_device, &deviceFeatures);

        VkPhysicalDeviceProperties devicePoperties{};
        vkGetPhysicalDeviceProperties(physical_device, &devicePoperties);

        queue_family_indices const indices{ get_physical_device_queue_family_indices(physical_device) };
        bool const supports_extensions{ check_device_extension_support(physical_device, required_extensions) };

        if(supports_extensions && indices.graphics != -1 && indices.compute != -1 && indices.transfer != -1) {
            switch(devicePoperties.deviceType) {
                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    score += 3;
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    score += 2;
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    score += 1;
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                    break;
            }
        } else {
            score = -1;
        }

        return score;
    }
}