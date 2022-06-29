#pragma once

#include "ember/graphics/push_constant_descriptor.hpp"

#include <ember/containers/array.hpp>

namespace ember::graphics {
    class shader;
    class descriptor_set_layout;
}

namespace ember::graphics {
    class compute_pipeline_object {
        //TYPES
    public:
        struct descriptor {
            shader const *shader{ nullptr };

            containers::array<descriptor_set_layout const *> descriptor_set_layouts{};
            containers::array<push_constant_descriptor> push_constants{};
        };

        //FUNCTIONS
    public:
        virtual ~compute_pipeline_object() = default;

        virtual descriptor const &get_descriptor() const = 0;
    };
}