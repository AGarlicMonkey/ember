#include "ember/graphics/graphics.hpp"

#include "vulkan/vulkan_instance.hpp"

#include <ember/core/log.hpp>

namespace ember::graphics {
    memory::unique_ptr<instance> create_instance(api api) {
        switch(api) {
            case api::vulkan:
                return create_vulkan_instance();
            default:
                EMBER_CHECK(false);
                return nullptr;
        }
    }
}