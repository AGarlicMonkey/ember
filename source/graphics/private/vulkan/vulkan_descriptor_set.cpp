#include "vulkan_descriptor_set.hpp"

#include "resource_cast.hpp"
#include "vulkan_buffer.hpp"
#include "vulkan_descriptor.hpp"
#include "vulkan_image.hpp"
#include "vulkan_image_view.hpp"
#include "vulkan_sampler.hpp"

namespace ember::inline graphics {
    void vulkan_descriptor_set::write(buffer const &buffer, std::size_t const offset, std::size_t const bytes, descriptor_type const type, std::uint32_t const slot) {
        VkBuffer const buffer_handle{ resource_cast<vulkan_buffer const>(&buffer)->get_handle() };

        VkDescriptorBufferInfo const buffer_info{
            .buffer = buffer_handle,
            .offset = offset,
            .range  = bytes,
        };

        VkWriteDescriptorSet const write_info{
            .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext            = nullptr,
            .dstSet           = handle,
            .dstBinding       = slot,
            .dstArrayElement  = 0,
            .descriptorCount  = 1,
            .descriptorType   = convert_descriptor_type(type),
            .pImageInfo       = nullptr,
            .pBufferInfo      = &buffer_info,
            .pTexelBufferView = nullptr,
        };

        vkUpdateDescriptorSets(device, 1, &write_info, 0, nullptr);
    }

    void vulkan_descriptor_set::write(image_view const &image_view, image::layout const layout, descriptor_type const type, std::uint32_t const slot) {
        VkImageView const image_view_handle{ resource_cast<vulkan_image_view const>(&image_view)->get_handle() };

        VkDescriptorImageInfo const image_info{
            .sampler     = VK_NULL_HANDLE,//Not using combined image samplers
            .imageView   = image_view_handle,
            .imageLayout = vulkan_image::convert_layout(layout),
        };

        VkWriteDescriptorSet const write_info{
            .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext            = nullptr,
            .dstSet           = handle,
            .dstBinding       = slot,
            .dstArrayElement  = 0,
            .descriptorCount  = 1,
            .descriptorType   = convert_descriptor_type(type),
            .pImageInfo       = &image_info,
            .pBufferInfo      = nullptr,
            .pTexelBufferView = nullptr,
        };

        vkUpdateDescriptorSets(device, 1, &write_info, 0, nullptr);
    }

    void vulkan_descriptor_set::write(sampler const &sampler, std::uint32_t const slot) {
        VkSampler const sampler_handle{ resource_cast<vulkan_sampler const>(&sampler)->get_handle() };

        VkDescriptorImageInfo const sampler_info{
            .sampler   = sampler_handle,
            .imageView = VK_NULL_HANDLE,//Not using combined image samplers
        };

        VkWriteDescriptorSet const write_info{
            .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext            = nullptr,
            .dstSet           = handle,
            .dstBinding       = slot,
            .dstArrayElement  = 0,
            .descriptorCount  = 1,
            .descriptorType   = VK_DESCRIPTOR_TYPE_SAMPLER,
            .pImageInfo       = &sampler_info,
            .pBufferInfo      = nullptr,
            .pTexelBufferView = nullptr,
        };

        vkUpdateDescriptorSets(device, 1, &write_info, 0, nullptr);
    }
}