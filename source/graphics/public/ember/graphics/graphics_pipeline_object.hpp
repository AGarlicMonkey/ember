#pragma once

#include "ember/graphics/push_constant_descriptor.hpp"

#include <cinttypes>
#include <ember/containers/array.hpp>
#include <ember/maths/vector.hpp>

namespace ember::inline graphics {
    class render_pass;
    class descriptor_set_layout;

    enum class vertex_attribute_format {
        R32_SFLOAT,
        R32G32_SFLOAT,
        R32G32B32_SFLOAT,
        R32G32B32A32_SFLOAT,
        R32G32B32A32_SINT,
    };

    /**
     * @brief Describes each vertex element.
     */
    struct vertex_input_binding_descriptor {
        std::uint32_t stride{ 0 }; /**< Size of each element. */
    };

    /**
     * @brief Describes each attribute of a vertex element.
     */
    struct vertex_attribute_descriptor {
        vertex_attribute_format format{};
        std::uint32_t offset{ 0 }; /**< Offset into the element of this attribute */
    };

    enum class element_state {
        fixed,
        dynamic,
    };

    enum class compare_op {
        less,
        less_or_equal,
    };

    struct area_descriptor {
        element_state state{ element_state::fixed }; /**< Dynamic states stop the pipeline object needing to be recreated if changed but require their data to be recorded everyframe. */
        vec2i position{ 0 };                         /**< Not required if state is dynamic. */
        vec2u size{ 0 };                             /**< Not required if state is dynamic. */
    };

    struct rasteriser_descriptor {
        //TODO
    };

    struct depth_state_descriptor {
        bool depth_test{ true };
        bool depth_write{ true };
        compare_op compare_op{ compare_op::less };
    };
}

namespace ember::inline graphics {
    /**
     * @brief Represents the state of the current graphics pipeline.
     */
    class graphics_pipeline_object {
        //TYPES
    public:
        struct descriptor {
            shader const *vertex_shader{ nullptr };
            shader const *pixel_shader{ nullptr };

            vertex_input_binding_descriptor vertex_input{};
            array<vertex_attribute_descriptor> vertex_attributes{}; /**< The index of each element maps to the layout(location = x) in the vertex shader. */

            area_descriptor viewport{};
            area_descriptor scissor{};

            rasteriser_descriptor rasteriser{};
            depth_state_descriptor depth_state{};

            bool enable_blending{ false };

            render_pass const *render_pass{ nullptr };

            array<descriptor_set_layout const *> descriptor_set_layouts{};
            array<push_constant_descriptor> push_constants{};
        };

        //FUNCTIONS
    public:
        virtual ~graphics_pipeline_object() = default;

        virtual descriptor const &get_descriptor() const = 0;
    };
}