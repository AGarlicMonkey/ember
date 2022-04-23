#include "vulkan_fence.hpp"

namespace ember::graphics {
    fence::descriptor const &vulkan_fence::get_descriptor() const {
        return desc;
    }

    void vulkan_fence::wait() {
        vkWaitForFences(device, 1, &handle, VK_TRUE, UINT64_MAX);
    }

    bool vulkan_fence::poll() {
        return vkGetFenceStatus(device, handle) == VK_SUCCESS;
    }

    void vulkan_fence::reset() {
        vkResetFences(device, 1, &handle);
    }
}