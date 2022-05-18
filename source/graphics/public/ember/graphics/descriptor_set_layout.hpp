#pragma once

#include "ember/graphics/descriptor.hpp"
#include "ember/graphics/shader.hpp"

#include <cinttypes>
#include <cstddef>
#include <ember/containers/array.hpp>

namespace ember::graphics {
    /**
     * @brief Describes a single binding within a descriptor set
     */
    struct descriptor_set_binding_info {
        std::uint32_t binding{ 0 };
        descriptor_type type{};
        std::size_t array_size{ 1 }; /**< If this descriptor represents an array of objects it'll be the size of the array, else 1. */
        shader::stage stage{};
    };
}

namespace ember::graphics {
    /**
     * @brief Describes the layout of an entire descriptor_set that will be bound into a pipeline object.
     * @details A 'set' is defined in the shader by: layout(set = 0, binding = 1). This
     * layout describes the type of binding in each binding slot for a given set. The index
     * of the set is the index in the array of descriptor_set_layouts given to the 
     * pipeline object's descriptor.
     * @see descriptor_set
     * @see descriptor_pool
     */
    class descriptor_set_layout {
        //VARIABLES
    public:
        struct descriptor {
            containers::array<descriptor_set_binding_info> bindings{};
        };

        //FUNCTIONS
    public:
        virtual ~descriptor_set_layout() = default;

        virtual descriptor const &get_descriptor() const = 0;
    };
}