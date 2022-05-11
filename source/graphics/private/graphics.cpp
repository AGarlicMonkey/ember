#include "ember/graphics/graphics.hpp"

#include "vulkan/vulkan_instance.hpp"

#include <ember/core/log.hpp>

EMBER_LOG_CATEGORY(EmberGraphics)

namespace ember::graphics {
    memory::unique_ptr<instance> create_instance(api api) {
#if EMBER_GRAPHICS_DEVICE_VALIDATION
        EMBER_LOG(EmberGraphics, log_level::debug, "Initialising with device validation enabled.");
#endif
#if EMBER_GRAPHICS_USAGE_VALIDATION
        EMBER_LOG(EmberGraphics, log_level::debug, "Initialising with API validation enabled.");
#endif
        switch(api) {
            case api::vulkan:
                return create_vulkan_instance();
            default:
                EMBER_CHECK(false);
                return nullptr;
        }
    }
}