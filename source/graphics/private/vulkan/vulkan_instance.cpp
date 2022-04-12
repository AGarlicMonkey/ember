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

#include "allocation_callbacks.hpp"
#include "exception.hpp"
#include "log.hpp"

#include <vector>

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

    bool check_validation_layer_support(std::vector<char const *> const &validation_layers) {
        std::uint32_t layer_count{ 0 };
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        std::vector<VkLayerProperties> available_layers(layer_count);
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
    vulkan_instance::~vulkan_instance() {
#if EMBER_GRAPHICS_DEVICE_VALIDATION
        destroy_debug_utils_messenger_EXT(instance, debug_messenger, &global_allocator);
#endif
        vkDestroyInstance(instance, &global_allocator);
    }

    memory::unique_ptr<vulkan_instance> create_vulkan_instance() {
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

        VkAllocationCallbacks instance_allocation_callbacks{ get_allocations_callbacks() };

        //TODO: Internal vector
        std::vector<char const *> instance_extensions {
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
        //TODO: Internal vector
        std::vector<char const *> const validation_layers{
            "VK_LAYER_KHRONOS_validation",
        };

        if(!check_validation_layer_support(validation_layers)) {
            EMBER_LOG(EmberGraphicsVulkan, log_level::warn, "Vulkan validation layers are not supported on this device. Unable to provide debugging information.");
        }
#endif

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

            EMBER_VULKAN_VERIFY_RESULT(vkCreateInstance(&create_info, &instance_allocation_callbacks, &instance), "Failed to create Vulkan instance.");

#if EMBER_GRAPHICS_DEVICE_VALIDATION
            if(create_debug_utils_messenger_EXT(instance, &debug_messenger_create_info, &instance_allocation_callbacks, &debug_messenger) < VK_SUCCESS) {
                EMBER_LOG(EmberGraphicsVulkan, log_level::warn, "Failed to create debug messenger. Debug info will be limited.");
            }
#endif

            EMBER_LOG(EmberGraphicsVulkan, log_level::info, "Creation of Vulkan instance was successful!");
            return memory::make_unique<vulkan_instance>(instance, debug_messenger, instance_allocation_callbacks);
        }
    }
}