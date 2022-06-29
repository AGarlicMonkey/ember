#include "vulkan_compute_pipeline_object.hpp"

namespace ember::graphics {
    compute_pipeline_object::descriptor const &vulkan_compute_pipeline_object::get_descriptor() const {
        return desc;
    }
}