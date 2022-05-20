#pragma once

#include "ember/graphics/sampler.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_sampler : public sampler {
        //VARIABLES
    private:
        descriptor desc{};

        VkSampler handle{ VK_NULL_HANDLE };
        VkDevice device{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_sampler() = delete;
        inline vulkan_sampler(descriptor desc, VkDevice device, VkSampler handle);

        vulkan_sampler(vulkan_sampler const &other) = delete;
        vulkan_sampler(vulkan_sampler &&other) noexcept = delete;

        vulkan_sampler &operator=(vulkan_sampler const &other) = delete;
        vulkan_sampler &operator=(vulkan_sampler &&other) noexcept = delete;

        inline ~vulkan_sampler() override;

        descriptor const &get_descriptor() const override;

        inline VkSampler get_handle() const;

        static VkFilter convert_filter(filter filter);
        static VkSamplerMipmapMode convert_mipmap_mode(filter filter);
        static VkSamplerAddressMode convert_address_mode(address_mode mode);
    };
}

#include "vulkan_sampler.inl"