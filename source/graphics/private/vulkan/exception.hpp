#pragma once

#include "log.hpp"

#include <ember/core/exception.hpp>

#if EMBER_CORE_ENABLE_EXCEPTIONS
    #include <string>
    #include <vulkan/vulkan.h>

namespace ember::graphics {
    std::string convert_result(VkResult const result) {
        switch(result) {
            case VK_ERROR_OUT_OF_HOST_MEMORY:
                return "Out of device memory.";
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                return "Out of device memory.";
            case VK_ERROR_INITIALIZATION_FAILED:
                return "Initialisation failed.";
            case VK_ERROR_DEVICE_LOST:
                return "Device lost.";
            case VK_ERROR_MEMORY_MAP_FAILED:
                return "Memory map failed.";
            case VK_ERROR_LAYER_NOT_PRESENT:
                return "Layer not present.";
            case VK_ERROR_EXTENSION_NOT_PRESENT:
                return "Extensions not present.";
            case VK_ERROR_FEATURE_NOT_PRESENT:
                return "Feature not present.";
            case VK_ERROR_INCOMPATIBLE_DRIVER:
                return "Incompatible driver.";
            case VK_ERROR_TOO_MANY_OBJECTS:
                return "Too many objects.";
            case VK_ERROR_FORMAT_NOT_SUPPORTED:
                return "Format not supported.";
            case VK_ERROR_FRAGMENTED_POOL:
                return "Fragmented pool.";
            case VK_ERROR_OUT_OF_POOL_MEMORY:
                return "Out of pool memory.";
            case VK_ERROR_INVALID_EXTERNAL_HANDLE:
                return "Invalid external handle.";
            case VK_ERROR_FRAGMENTATION:
                return "Fragmentation.";
            case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
                return "Invalid opaque capture address.";
            case VK_ERROR_SURFACE_LOST_KHR:
                return "Surface lost.";
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
                return "Native window in use.";
            case VK_SUBOPTIMAL_KHR:
                return "Suboptimal.";
            case VK_ERROR_OUT_OF_DATE_KHR:
                return "Out of date.";
            case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
                return "Incompatible display.";
            case VK_ERROR_UNKNOWN:
            default:
                return "Reason unknown.";
        }
    }
}

    #define EMBER_VULKAN_VERIFY_RESULT(function, message)                                                                                             \
        {                                                                                                                                             \
            if(VkResult const result{ function }; result < VK_SUCCESS) {                                                                              \
                std::string const error_message{ convert_result(result) };                                                                            \
                EMBER_LOG(EmberGraphicsVulkan, ::ember::log_level::critical, "Vulkan call failed: {0}. {1} {2}", #function, message, error_message); \
                EMBER_THROW(::ember::exception{ "Vulkan error. See log for details." });                                                              \
            }                                                                                                                                         \
        }
#else
    #define EMBER_VULKAN_VERIFY_RESULT(function, message) function
#endif