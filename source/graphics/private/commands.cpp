#include "commands.hpp"

#include <ember/core/log.hpp>

namespace {
    template<ember::command_type type>
    void do_destruct(std::byte *const command_memory) {
        reinterpret_cast<ember::recorded_command<type> *>(command_memory)->~recorded_command<type>();
    }
}

namespace ember::inline graphics {
    std::size_t get_size_of_command(command_type const type) {
        switch(type) {
            // clang-format off
            case command_type::copy_buffer_to_buffer_command:           return sizeof(recorded_command<command_type::copy_buffer_to_buffer_command>);
            case command_type::copy_buffer_to_image_command:            return sizeof(recorded_command<command_type::copy_buffer_to_image_command>);
            case command_type::copy_image_to_buffer_command:            return sizeof(recorded_command<command_type::copy_image_to_buffer_command>);
            case command_type::execution_barrier_command:               return sizeof(recorded_command<command_type::execution_barrier_command>);
            case command_type::buffer_memory_barrier_command:           return sizeof(recorded_command<command_type::buffer_memory_barrier_command>);
            case command_type::image_memory_barrier_command:            return sizeof(recorded_command<command_type::image_memory_barrier_command>);
#if EMBER_GRAPHICS_ENABLE_USER_MARKERS
            case command_type::push_user_marker_command:                return sizeof(recorded_command<command_type::push_user_marker_command>);
            case command_type::pop_user_marker_command:                 return sizeof(recorded_command<command_type::pop_user_marker_command>);
#endif
            case command_type::bind_compute_pipeline_object_command:    return sizeof(recorded_command<command_type::bind_compute_pipeline_object_command>);
            case command_type::bind_descriptor_set_command:             return sizeof(recorded_command<command_type::bind_descriptor_set_command>);
            case command_type::push_constant_command:                   return sizeof(recorded_command<command_type::push_constant_command>);
            case command_type::dispatch_command:                        return sizeof(recorded_command<command_type::dispatch_command>);
            case command_type::dispatch_indirect_command:               return sizeof(recorded_command<command_type::dispatch_indirect_command>);
            case command_type::begin_render_pass_command:               return sizeof(recorded_command<command_type::begin_render_pass_command>);
            case command_type::end_render_pass_command:                 return sizeof(recorded_command<command_type::end_render_pass_command>);
            case command_type::set_viewport_command:                    return sizeof(recorded_command<command_type::set_viewport_command>);
            case command_type::set_scissor_command:                     return sizeof(recorded_command<command_type::set_scissor_command>);
            case command_type::bind_graphics_pipeline_object_command:   return sizeof(recorded_command<command_type::bind_graphics_pipeline_object_command>);
            case command_type::bind_vertex_buffer_command:              return sizeof(recorded_command<command_type::bind_vertex_buffer_command>);
            case command_type::bind_index_buffer_command:               return sizeof(recorded_command<command_type::bind_index_buffer_command>);
            case command_type::draw_indexed_command:                    return sizeof(recorded_command<command_type::draw_indexed_command>);
            case command_type::draw_indexed_indirect_command:           return sizeof(recorded_command<command_type::draw_indexed_indirect_command>);
            default: EMBER_CHECK(false); return 0;
                // clang-format on
        }
    }

    void destruct_command(command_type const type, std::byte *const command_memory) {
        switch(type) {
            // clang-format off
            case command_type::copy_buffer_to_buffer_command:           do_destruct<command_type::copy_buffer_to_buffer_command>(command_memory); break;
            case command_type::copy_buffer_to_image_command:            do_destruct<command_type::copy_buffer_to_image_command>(command_memory); break;
            case command_type::copy_image_to_buffer_command:            do_destruct<command_type::copy_image_to_buffer_command>(command_memory); break;
            case command_type::execution_barrier_command:               do_destruct<command_type::execution_barrier_command>(command_memory); break;
            case command_type::buffer_memory_barrier_command:           do_destruct<command_type::buffer_memory_barrier_command>(command_memory); break;
            case command_type::image_memory_barrier_command:            do_destruct<command_type::image_memory_barrier_command>(command_memory); break;
#if EMBER_GRAPHICS_ENABLE_USER_MARKERS
            case command_type::push_user_marker_command:                do_destruct<command_type::push_user_marker_command>(command_memory); break;
            case command_type::pop_user_marker_command:                 do_destruct<command_type::pop_user_marker_command>(command_memory); break;
#endif
            case command_type::bind_compute_pipeline_object_command:    do_destruct<command_type::bind_compute_pipeline_object_command>(command_memory); break;
            case command_type::bind_descriptor_set_command:             do_destruct<command_type::bind_descriptor_set_command>(command_memory); break;
            case command_type::push_constant_command:                   do_destruct<command_type::push_constant_command>(command_memory); break;
            case command_type::dispatch_command:                        do_destruct<command_type::dispatch_command>(command_memory); break;
            case command_type::dispatch_indirect_command:               do_destruct<command_type::dispatch_indirect_command>(command_memory); break;
            case command_type::begin_render_pass_command:               do_destruct<command_type::begin_render_pass_command>(command_memory); break;
            case command_type::end_render_pass_command:                 do_destruct<command_type::end_render_pass_command>(command_memory); break;
            case command_type::set_viewport_command:                    do_destruct<command_type::set_viewport_command>(command_memory); break;
            case command_type::set_scissor_command:                     do_destruct<command_type::set_scissor_command>(command_memory); break;
            case command_type::bind_graphics_pipeline_object_command:   do_destruct<command_type::bind_graphics_pipeline_object_command>(command_memory); break;
            case command_type::bind_vertex_buffer_command:              do_destruct<command_type::bind_vertex_buffer_command>(command_memory); break;
            case command_type::bind_index_buffer_command:               do_destruct<command_type::bind_index_buffer_command>(command_memory); break;
            case command_type::draw_indexed_command:                    do_destruct<command_type::draw_indexed_command>(command_memory); break;
            case command_type::draw_indexed_indirect_command:           do_destruct<command_type::draw_indexed_indirect_command>(command_memory); break;
            default: EMBER_CHECK(false);
                // clang-format on
        }
    }
}