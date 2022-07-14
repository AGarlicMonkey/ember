#pragma once

#include "ember/graphics/shader_cache.hpp"
#include "vulkan_shader.hpp"

#include <ember/memory/unique_ptr.hpp>
#include <ember/containers/map.hpp>
#include <vulkan/vulkan.h>

namespace ember::inline graphics {
    class vulkan_shader;
}

namespace ember::inline graphics {
    class vulkan_shader_cache : public shader_cache {
        //VARIABLES
    private:
        VkDevice device{ VK_NULL_HANDLE };

        map<std::string, unique_ptr<vulkan_shader>> compiled_modules{};

        //FUNCTIONS
    public:
        vulkan_shader_cache() = delete;
        inline vulkan_shader_cache(VkDevice device);

        vulkan_shader_cache(vulkan_shader_cache const &other) = delete;
        inline vulkan_shader_cache(vulkan_shader_cache &&other) noexcept;

        vulkan_shader_cache &operator=(vulkan_shader_cache const &other) = delete;
        inline vulkan_shader_cache &operator=(vulkan_shader_cache &&other) noexcept;

        inline ~vulkan_shader_cache() override;

        shader *get_shader(std::string const &shader_name) override;
    };
}

#include "vulkan_shader_cache.inl"