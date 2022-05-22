#pragma once

#include "ember/graphics/descriptor_set.hpp"

#include <vulkan/vulkan.h>

namespace ember::graphics {
    class vulkan_descriptor_set : public descriptor_set {
        //VARIABLES
    private:
        VkDevice device{ VK_NULL_HANDLE };
        VkDescriptorSet handle{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        vulkan_descriptor_set() = delete;
        inline vulkan_descriptor_set(VkDevice device, VkDescriptorSet handle);

        vulkan_descriptor_set(vulkan_descriptor_set const &other)     = delete;
        vulkan_descriptor_set(vulkan_descriptor_set &&other) noexcept = delete;

        vulkan_descriptor_set &operator=(vulkan_descriptor_set const &other) = delete;
        vulkan_descriptor_set &operator=(vulkan_descriptor_set &&other) noexcept = delete;

        inline ~vulkan_descriptor_set() override;

        void write(buffer const &buffer, std::size_t const offset, std::size_t const bytes, descriptor_type const type, std::uint32_t const slot) override;
        void write(image_view const &image_view, image::layout const layout, descriptor_type const type, std::uint32_t const slot) override;
        void write(sampler const &sampler, std::uint32_t const slot) override;

        inline VkDescriptorSet get_handle() const;
    };
}

#include "vulkan_descriptor_set.inl"