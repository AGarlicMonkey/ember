#if EMBER_PLATFORM_WIN32
    #define VK_USE_PLATFORM_WIN32_KHR
#elif EMBER_PLATFORM_MACOS
    #error vulkan not supported on macos
#elif EMBER_PLATFORM_XLIB
    #error vulkan not supported on linux
#endif

// clang-format off
//these 2 headers need to be defined in this order
#include <vulkan/vk_sdk_platform.h>
#include <vulkan/vulkan.h>
// clang-format on

#include "vulkan_instance.hpp"

#include "host_memory_allocator.hpp"
#include "verification.hpp"
#include "log.hpp"
#include "vulkan_device.hpp"

#include <map>
#include <set>

using namespace ember::containers;
using namespace ember::memory;

EMBER_LOG_CATEGORY(Vulkan)

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messagType,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
    void *pUserData) {
    if((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) != 0) {
        EMBER_LOG(Vulkan, ember::log_level::trace, pCallbackData->pMessage);
    } else if((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) != 0) {
        EMBER_LOG(Vulkan, ember::log_level::warn, pCallbackData->pMessage);
    } else if((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) != 0) {
        EMBER_LOG(Vulkan, ember::log_level::error, pCallbackData->pMessage);
    }

    return VK_FALSE;
}

namespace {
    VkResult create_debug_utils_messenger_EXT(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo, VkAllocationCallbacks const *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
        if(func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void destroy_debug_utils_messenger_EXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator) {
        auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
        if(func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    bool check_validation_layer_support(array<char const *> const &validation_layers) {
        std::uint32_t layer_count{ 0 };
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        array<VkLayerProperties> available_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

        for(char const *layerName : validation_layers) {
            bool layerFound = false;

            for(auto const &layer_properties : available_layers) {
                if(std::strcmp(layerName, layer_properties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if(!layerFound) {
                return false;
            }
        }

        return true;
    }
}

namespace ember::graphics {
    vulkan_instance::vulkan_instance(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, unique_ptr<vulkan_device> device)
        : instance{ instance }
        , debug_messenger{ debug_messenger }
        , device{ std::move(device) } {
    }

    vulkan_instance::vulkan_instance(vulkan_instance &&other) noexcept = default;

    vulkan_instance &vulkan_instance::operator=(vulkan_instance &&other) noexcept = default;

    vulkan_instance::~vulkan_instance() {
        //Reset the physical device before destroying the instance
        device.reset();

#if EMBER_GRAPHICS_DEVICE_VALIDATION
        destroy_debug_utils_messenger_EXT(instance, debug_messenger, &global_host_allocation_callbacks);
#endif
        vkDestroyInstance(instance, &global_host_allocation_callbacks);
    }

    device *vulkan_instance::get_device() const noexcept {
        return device.get();
    }

    unique_ptr<vulkan_instance> create_vulkan_instance() {
        EMBER_LOG(EmberGraphicsVulkan, log_level::debug, "Creating vulkan graphics instance...");

#if EMBER_CORE_ENABLE_LOGGING
        {
            std::uint32_t extension_count{ 0 };
            vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
            std::vector<VkExtensionProperties> extensions(extension_count);//TODO: Internal vector
            vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

            EMBER_LOG(EmberGraphicsVulkan, log_level::trace, "Available instance extensions:");
            for(auto const &extension : extensions) {
                EMBER_LOG(EmberGraphicsVulkan, log_level::trace, "\t{0}", extension.extensionName);
            }
        }
#endif

        array<char const *> instance_extensions {
            VK_KHR_SURFACE_EXTENSION_NAME,

#if EMBER_PLATFORM_WIN32
                VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif
#elif EMBER_PLATFORM_XLIB
//TODO
#endif

#if EMBER_GRAPHICS_DEVICE_VALIDATION
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
        };

#if EMBER_GRAPHICS_DEVICE_VALIDATION
        array<char const *> const validation_layers{
            "VK_LAYER_KHRONOS_validation",
        };

        if(!check_validation_layer_support(validation_layers)) {
            EMBER_LOG(EmberGraphicsVulkan, log_level::warn, "Vulkan validation layers are not supported on this device. Unable to provide debugging information.");
        }
#endif

        //Create instance
        VkInstance instance{ VK_NULL_HANDLE };
        VkDebugUtilsMessengerEXT debug_messenger{ VK_NULL_HANDLE };
        {
            VkApplicationInfo const app_info{
                .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName   = "",                      //TODO: Get app name
                .applicationVersion = VK_MAKE_VERSION(1, 0, 0),//TODO: Get app version
                .pEngineName        = "ember",
                .engineVersion      = VK_MAKE_VERSION(1, 0, 0),//TODO: Get engine version
                .apiVersion         = VK_API_VERSION_1_2,
            };

#if EMBER_GRAPHICS_DEVICE_VALIDATION
            VkDebugUtilsMessengerCreateInfoEXT const debug_messenger_create_info{
                .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                .pfnUserCallback = debug_callback,
                .pUserData       = nullptr,
            };
#endif

            VkInstanceCreateInfo const create_info {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#if EMBER_GRAPHICS_DEVICE_VALIDATION
                .pNext               = &debug_messenger_create_info,
                .pApplicationInfo    = &app_info,
                .enabledLayerCount   = static_cast<std::uint32_t>(validation_layers.size()),
                .ppEnabledLayerNames = validation_layers.data(),
#else
                .pNext               = nullptr,
                .pApplicationInfo    = &app_info,
                .enabledLayerCount   = 0,
                .ppEnabledLayerNames = nullptr,
#endif
                .enabledExtensionCount   = static_cast<std::uint32_t>(instance_extensions.size()),
                .ppEnabledExtensionNames = instance_extensions.data(),
            };

            EMBER_VULKAN_VERIFY_RESULT(vkCreateInstance(&create_info, &global_host_allocation_callbacks, &instance), "Failed to create Vulkan instance.");

#if EMBER_GRAPHICS_DEVICE_VALIDATION
            if(create_debug_utils_messenger_EXT(instance, &debug_messenger_create_info, &global_host_allocation_callbacks, &debug_messenger) < VK_SUCCESS) {
                EMBER_LOG(EmberGraphicsVulkan, log_level::warn, "Failed to create debug messenger. Debug info will be limited.");
            }
#endif
        }

        array<char const *> required_device_extensions {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,

#if EMBER_GRAPHICS_TRACK_MEMORY
                VK_EXT_MEMORY_BUDGET_EXTENSION_NAME,
#endif
        };

        //Select physical device to use for rendering
        VkPhysicalDevice physical_device{ VK_NULL_HANDLE };
        {
            EMBER_LOG(EmberGraphicsVulkan, log_level::trace, "Querying devices...");

            std::uint32_t device_count{ 0 };
            vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
            EMBER_THROW_IF_FAILED(device_count > 0, graphics_exception{ "No devices available. Cannot continue with vulkan initialisation" });
            array<VkPhysicalDevice> physical_devices(device_count);
            vkEnumeratePhysicalDevices(instance, &device_count, physical_devices.data());

#if EMBER_CORE_ENABLE_LOGGING
            EMBER_LOG(EmberGraphicsVulkan, log_level::trace, "Found {0} total devices:", device_count);
            for(auto const &physical_device : physical_devices) {
                VkPhysicalDeviceProperties device_properties{};
                vkGetPhysicalDeviceProperties(physical_device, &device_properties);
                EMBER_LOG(EmberGraphicsVulkan, log_level::trace, "\t{0}", device_properties.deviceName);
            }
#endif

            //Score avilable devices. Anything below 0 is considered in-complete and shouldn't be selected
            std::map<std::int32_t, VkPhysicalDevice> device_Scores{};//TODO: custom map
            for(auto const &physical_device : physical_devices) {
                device_Scores[vulkan_device::score_physical_device(physical_device, required_device_extensions)] = physical_device;
            }
            if(device_Scores.rbegin()->first > 0) {
                physical_device = device_Scores.rbegin()->second;
            }
            EMBER_THROW_IF_FAILED(physical_device != VK_NULL_HANDLE, device_selection_failed_exception{ "Failed to create initialise Vulkan. Could not find a suitable device." });

#if EMBER_CORE_ENABLE_LOGGING
            {
                VkPhysicalDeviceProperties device_poperties{};
                vkGetPhysicalDeviceProperties(physical_device, &device_poperties);
                EMBER_LOG(EmberGraphicsVulkan, log_level::debug, "Selecting '{0}'", device_poperties.deviceName);
            }

            {
                std::uint32_t queue_family_count{ 0 };
                vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
                array<VkQueueFamilyProperties> queue_families(queue_family_count);
                vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

                auto const queue_family_indices{ vulkan_device::get_physical_device_queue_family_indices(physical_device) };

                EMBER_LOG(EmberGraphicsVulkan, log_level::trace, "Queue family properties:");
                EMBER_LOG(EmberGraphicsVulkan, log_level::trace, "\tGraphics:\tid: {0}, count: {1}", queue_family_indices.graphics, queue_families[queue_family_indices.graphics].queueCount);
                EMBER_LOG(EmberGraphicsVulkan, log_level::trace, "\tCompute:\tid: {0}, count: {1}", queue_family_indices.compute, queue_families[queue_family_indices.compute].queueCount);
                EMBER_LOG(EmberGraphicsVulkan, log_level::trace, "\tTransfer:\tid: {0}, count: {1}", queue_family_indices.transfer, queue_families[queue_family_indices.transfer].queueCount);
            }

            {
                uint32_t extension_count{ 0 };
                vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);
                std::vector<VkExtensionProperties> extensions(extension_count);
                vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, extensions.data());

                EMBER_LOG(EmberGraphicsVulkan, log_level::trace, "Available device extensions:");
                for(auto const &extension : extensions) {
                    EMBER_LOG(EmberGraphicsVulkan, log_level::trace, "\t{0}", extension.extensionName);
                }
            }
#endif
        }

        //Create the logical device to wrap the physical one
        VkDevice logical_device{ VK_NULL_HANDLE };
        auto const queue_family_indices{ vulkan_device::get_physical_device_queue_family_indices(physical_device) };
        unique_ptr<vulkan_device> selected_device{};
        {
            //TODO: use custom set
            std::set<std::uint32_t> unique_family_indices{
                queue_family_indices.graphics,
                queue_family_indices.compute,
                queue_family_indices.transfer,
            };

            float constexpr queue_priority{ 1.0f };
            array<VkDeviceQueueCreateInfo> queue_create_infos{};
            for(std::uint32_t queueFamily : unique_family_indices) {
                VkDeviceQueueCreateInfo queue_create_info{
                    .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .queueFamilyIndex = queueFamily,
                    .queueCount       = 1,
                    .pQueuePriorities = &queue_priority,
                };
                queue_create_infos.push_back(queue_create_info);
            }

            VkPhysicalDeviceFeatures device_features{
                .imageCubeArray    = VK_TRUE,
                .samplerAnisotropy = VK_TRUE,
            };

            VkDeviceCreateInfo create_info{
                .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .queueCreateInfoCount    = static_cast<std::uint32_t>(queue_create_infos.size()),
                .pQueueCreateInfos       = queue_create_infos.data(),
                .enabledLayerCount       = 0,
                .enabledExtensionCount   = static_cast<std::uint32_t>(required_device_extensions.size()),
                .ppEnabledExtensionNames = required_device_extensions.data(),
                .pEnabledFeatures        = &device_features,
            };

            EMBER_VULKAN_VERIFY_RESULT(vkCreateDevice(physical_device, &create_info, &global_host_allocation_callbacks, &logical_device), "Failed to create logical device for vulkan. ");
            selected_device = make_unique<vulkan_device>(instance, physical_device, logical_device, queue_family_indices);
        }

        EMBER_LOG(EmberGraphicsVulkan, log_level::info, "Creation of Vulkan instance was successful!");
        return make_unique<vulkan_instance>(instance, debug_messenger, std::move(selected_device));
    }
}