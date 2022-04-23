#pragma once

#include <cinttypes>

namespace ember::graphics {
    enum class command_type : std::uint8_t {
        //General
#if EMBER_GRAPHICS_DEBUG_UTILITIES
        push_user_marker_command,
        pop_user_marker_command,
#endif

        //Transfer
        copy_buffer_to_buffer_command,
        copy_buffer_to_image_command,
        copy_image_to_buffer_command,
        buffer_memory_barrier_command,
        image_memory_barrier_command,

        //Compute
        bind_compute_pipeline_object_command,
        bind_descriptor_set_command,
        push_constant_command,
        dispatch_command,

        //Graphics
        begin_render_pass_command,
        end_render_pass_command,
        set_viewport_command,
        set_scissor_command,
        bind_graphics_pipeline_object_command,
        bind_vertex_buffer_command,
        bind_index_buffer_command,
        draw_indexed_command,
        present_command,
    };

    template<command_type type>
    struct recorded_command;
}