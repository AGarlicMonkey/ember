#include "ember/graphics/compute_command_buffer.hpp"

#include "commands.hpp"

namespace ember::inline graphics {
#if EMBER_GRAPHICS_ENABLE_USER_MARKERS
    void compute_command_buffer::push_user_marker(std::string name, vec4f const colour) {
        record_command<command_type::push_user_marker_command>(std::move(name), colour);
    }

    void compute_command_buffer::pop_user_marker() {
        record_command<command_type::pop_user_marker_command>();
    }
#endif

    void compute_command_buffer::bind_pipeline_object(compute_pipeline_object const *const pipeline_object) {
        record_command<command_type::bind_compute_pipeline_object_command>(pipeline_object);
    }

    void compute_command_buffer::bind_descriptor_set(descriptor_set const *const descriptor_set, std::uint32_t const set_num) {
        record_command<command_type::bind_descriptor_set_command>(descriptor_set, set_num);
    }

    void compute_command_buffer::push_constant(shader::stage const stage, std::size_t const offset, std::size_t const bytes, void const *const data) {
        record_command<command_type::push_constant_command>(stage, offset, bytes, data);
    }

    void compute_command_buffer::dispatch(vec3u const group_count) {
        record_command<command_type::dispatch_command>(group_count);
    }

    void compute_command_buffer::dispatch_indirect(buffer const *const indirect_buffer, std::size_t const offset) {
        record_command<command_type::dispatch_indirect_command>(indirect_buffer, offset);
    }
}