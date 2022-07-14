#include "vulkan_framebuffer.hpp"

namespace ember::inline graphics {
    framebuffer::descriptor const &vulkan_framebuffer::get_descriptor() const {
        return desc;
    }
}