#include "vulkan_shader.hpp"

namespace ember::graphics {
    VkShaderStageFlags vulkan_shader::convert_stage(stage const stage) {
        VkShaderStageFlags flags{ 0 };
        if((stage & stage::vertex) != 0) {
            flags |= VK_SHADER_STAGE_VERTEX_BIT;
        }
        if((stage & stage::pixel) != 0) {
            flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        if((stage & stage::compute) != 0) {
            flags |= VK_SHADER_STAGE_COMPUTE_BIT;
        }
        return flags;
    }
}