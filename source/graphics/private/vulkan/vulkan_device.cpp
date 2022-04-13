#include "vulkan_device.hpp"

#include <ember/containers/array.hpp>
#include <set>

using namespace ember::containers;

namespace {
    bool check_device_extension_support(VkPhysicalDevice device, array<char const *> const &extensions) {
        uint32_t extension_count{ 0 };
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
        array<VkExtensionProperties> available_extensions(extension_count);
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

namespace ember::graphics {
    vulkan_device::queue_family_indices vulkan_device::get_physical_device_queue_family_indices(VkPhysicalDevice device) {
        queue_family_indices indices{};
        //TODO: custom set
        std::set<std::uint32_t> graphics_families{};
        std::set<std::uint32_t> compute_families{};
        std::set<std::uint32_t> transfer_families{};

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

        indices.graphics_family = *graphics_families.begin();

        //Try and make compute and transfer different families if possible
        if(compute_families.size() > 1) {
            compute_families.erase(indices.graphics_family);
        }
        if(transfer_families.size() > 1) {
            transfer_families.erase(indices.graphics_family);
        }

        indices.compute_family = *compute_families.begin();

        if(transfer_families.size() > 1) {
            transfer_families.erase(indices.compute_family);
        }

        indices.transfer_family = *transfer_families.begin();

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

        if(supports_extensions && indices.graphics_family != -1 && indices.compute_family != -1 && indices.transfer_family != -1) {
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