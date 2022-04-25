#include "commands.hpp"

#include <ember/core/log.hpp>

namespace ember::graphics {
    std::size_t get_size_of_command(command_type const type) {
        switch(type) {
            case command_type::copy_buffer_to_buffer_command:
                return sizeof(recorded_command<command_type::copy_buffer_to_buffer_command>);
            case command_type::copy_buffer_to_image_command:
                return sizeof(recorded_command<command_type::copy_buffer_to_image_command>);
            case command_type::copy_image_to_buffer_command:
                return sizeof(recorded_command<command_type::copy_image_to_buffer_command>);
            case command_type::buffer_memory_barrier_command:
                return sizeof(recorded_command<command_type::buffer_memory_barrier_command>);
            case command_type::image_memory_barrier_command:
                return sizeof(recorded_command<command_type::image_memory_barrier_command>);
#if EMBER_GRAPHICS_DEBUG_UTILITIES || EMBER_CORE_ENABLE_PROFILING
            case command_type::push_user_marker_command:
                return sizeof(recorded_command<command_type::push_user_marker_command>);
            case command_type::pop_user_marker_command:
                return sizeof(recorded_command<command_type::pop_user_marker_command>);
#endif
            case command_type::bind_compute_pipeline_object_command:
                return sizeof(recorded_command<command_type::bind_compute_pipeline_object_command>);
            case command_type::bind_descriptor_set_command:
                return sizeof(recorded_command<command_type::bind_descriptor_set_command>);
            case command_type::push_constant_command:
                return sizeof(recorded_command<command_type::push_constant_command>);
            case command_type::dispatch_command:
                return sizeof(recorded_command<command_type::dispatch_command>);
            case command_type::begin_render_pass_command:
                return sizeof(recorded_command<command_type::begin_render_pass_command>);
            case command_type::end_render_pass_command:
                return sizeof(recorded_command<command_type::end_render_pass_command>);
            case command_type::set_viewport_command:
                return sizeof(recorded_command<command_type::set_viewport_command>);
            case command_type::set_scissor_command:
                return sizeof(recorded_command<command_type::set_scissor_command>);
            case command_type::bind_graphics_pipeline_object_command:
                return sizeof(recorded_command<command_type::bind_graphics_pipeline_object_command>);
            case command_type::bind_vertex_buffer_command:
                return sizeof(recorded_command<command_type::bind_vertex_buffer_command>);
            case command_type::bind_index_buffer_command:
                return sizeof(recorded_command<command_type::bind_index_buffer_command>);
            case command_type::draw_indexed_command:
                return sizeof(recorded_command<command_type::draw_indexed_command>);
            default:
                EMBER_CHECK(false);
                return 0;
        }
    }
}