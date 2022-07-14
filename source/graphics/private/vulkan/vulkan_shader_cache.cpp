#include "vulkan_shader_cache.hpp"

#include "host_memory_allocator.hpp"
#include "verification.hpp"

namespace ember::inline graphics {
    shader *vulkan_shader_cache::get_shader(std::string const &shader_name) {
        if(!compiled_modules.contains(shader_name)) {
            array<std::uint32_t> const shader_spirv{ get_spriv_for_shader(shader_name) };

            VkShaderModuleCreateInfo const create_info{
                .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                .pNext    = nullptr,
                .flags    = 0,
                .codeSize = shader_spirv.size() * sizeof(std::uint32_t),
                .pCode    = shader_spirv.data(),
            };

            VkShaderModule module{ VK_NULL_HANDLE };
            EMBER_VULKAN_VERIFY_RESULT(vkCreateShaderModule(device, &create_info, &global_host_allocation_callbacks, &module), "Failed to create VkShaderModule.");

            compiled_modules[shader_name] = make_unique<vulkan_shader>(device, module);
        }

        return compiled_modules.at(shader_name).get();
    }
}