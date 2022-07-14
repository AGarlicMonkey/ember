#pragma once

#include "ember/graphics/descriptor.hpp"

#include <vulkan/vulkan.h>

namespace ember::inline graphics {
    VkDescriptorType convert_descriptor_type(descriptor_type const type);
}