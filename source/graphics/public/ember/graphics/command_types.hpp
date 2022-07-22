#pragma once

#include <cinttypes>
#include <ember/maths/vector.hpp>

#define EMBER_GRAPHICS_ENABLE_USER_MARKERS (EMBER_GRAPHICS_DEBUG_UTILITIES || EMBER_CORE_ENABLE_PROFILING)

namespace ember::inline graphics {
    enum class command_type : std::uint8_t {
        //Transfer
        copy_buffer_to_buffer_command,
        copy_buffer_to_image_command,
        copy_image_to_buffer_command,
        execution_barrier_command,
        buffer_memory_barrier_command,
        image_memory_barrier_command,

        //Compute
#if EMBER_GRAPHICS_ENABLE_USER_MARKERS
        push_user_marker_command,
        pop_user_marker_command,
#endif
        bind_compute_pipeline_object_command,
        bind_descriptor_set_command,
        push_constant_command,
        dispatch_command,
        dispatch_indirect_command,

        //Graphics
        begin_render_pass_command,
        end_render_pass_command,
        set_viewport_command,
        set_scissor_command,
        bind_graphics_pipeline_object_command,
        bind_vertex_buffer_command,
        bind_index_buffer_command,
        draw_indexed_command,
        draw_indexed_indirect_command,
    };

    struct dispatch_indirect_info {
        std::uint32_t x{ 1 }; /**< How many groups on the x axis to dispatch. */
        std::uint32_t y{ 1 }; /**< How many groups on the y axis to dispatch. */
        std::uint32_t z{ 1 }; /**< How many groups on the z axis to dispatch. */
    };

    struct draw_indexed_indirect_info {
        std::uint32_t index_count{ 0 };    /**< How many indices from the bound index buffer to draw. */
        std::uint32_t instance_count{ 1 }; /**< How many instances to draw from this one draw call. */
        std::uint32_t first_index{ 0 };    /**< First index in the index buffer to use. */
        std::int32_t vertex_offset{ 0 };   /**< Added to the vertex index before indexing into the vertex buffer. */
        std::uint32_t first_instance{ 0 }; /**< The ID of the first instance of this draw call. */
    };

    struct command {
        command *next{ nullptr };
    };

    template<command_type type>
    struct recorded_command;
}