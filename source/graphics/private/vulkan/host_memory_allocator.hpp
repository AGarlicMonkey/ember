#pragma once

#include <cstddef>
#include <vulkan/vulkan.h>

namespace ember::graphics {
    void *alloc(void *user_data, std::size_t size, std::size_t alignment, VkSystemAllocationScope allocation_scope);
    void *realloc(void *user_data, void *original, std::size_t size, std::size_t alignment, VkSystemAllocationScope allocation_scope);
    void free(void *user_data, void *memory);

    inline static VkAllocationCallbacks const global_host_allocation_callbacks{
        .pUserData             = nullptr,
        .pfnAllocation         = alloc,
        .pfnReallocation       = realloc,
        .pfnFree               = free,
        .pfnInternalAllocation = nullptr,
        .pfnInternalFree       = nullptr,
    };
}