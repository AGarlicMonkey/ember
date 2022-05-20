#include "host_memory_allocator.hpp"

namespace ember::graphics {
    vulkan_sampler::vulkan_sampler(descriptor desc, VkDevice device, VkSampler handle)
        : desc{ desc }
        , device{ device }
        , handle{ handle } {
    }

    vulkan_sampler::vulkan_sampler(vulkan_sampler &&other) noexcept = default;

    vulkan_sampler &vulkan_sampler::operator=(vulkan_sampler &&other) noexcept = default;

    vulkan_sampler::~vulkan_sampler() {
        vkDestroySampler(device, handle, &global_host_allocation_callbacks);
    }

    VkSampler vulkan_sampler::get_handle() const {
        return handle;
    }
}