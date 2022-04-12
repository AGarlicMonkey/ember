namespace ember::graphics {
    VkAllocationCallbacks get_allocations_callbacks() {
        return VkAllocationCallbacks{
            .pUserData             = nullptr,
            .pfnAllocation         = alloc,
            .pfnReallocation       = realloc,
            .pfnFree               = free,
            .pfnInternalAllocation = nullptr,
            .pfnInternalFree       = nullptr,
        };
    }
}