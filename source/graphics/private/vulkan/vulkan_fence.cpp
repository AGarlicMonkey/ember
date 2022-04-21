#include "vulkan_fence.hpp"

namespace ember::graphics {
    fence::descriptor const &vulkan_fence::get_descriptor() const {
        return desc;
    }

    void vulkan_fence::wait() {
        vkWaitForFences(device, 1, &handle, VK_TRUE, UINT64_MAX);
    }

    void vulkan_fence::reset() {
        vkResetFences(device, 1, &handle);
    }
}