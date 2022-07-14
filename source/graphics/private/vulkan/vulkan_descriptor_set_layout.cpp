#include "vulkan_descriptor_set_layout.hpp"

#include <ember/core/log.hpp>

namespace ember::inline graphics {
    descriptor_set_layout::descriptor const &vulkan_descriptor_set_layout::get_descriptor() const {
        return desc;
    }
}