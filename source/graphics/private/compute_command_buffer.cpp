#include "ember/graphics/compute_command_buffer.hpp"

#include "commands.hpp"

using namespace ember::maths;

namespace ember::graphics {
#if EMBER_GRAPHICS_ENABLE_USER_MARKERS
    void compute_command_buffer::push_user_marker(std::string name, vec4f const colour) {
        record_command<command_type::push_user_marker_command>(std::move(name), colour);
    }

    void compute_command_buffer::pop_user_marker() {
        record_command<command_type::pop_user_marker_command>();
    }
#endif

    void compute_command_buffer::bind_pipeline_object(compute_pipeline_object const *const pipeline_object) {
        //TODO
        EMBER_CHECK(false);
    }

    void compute_command_buffer::bind_descriptor_set(descriptor_set const *const descriptor_set, std::uint32_t const set_num) {
        record_command<command_type::bind_descriptor_set_command>(descriptor_set, set_num);
    }

    void compute_command_buffer::push_constant(shader::stage const stage, std::size_t const offset, std::size_t const bytes, void const *const data) {
        record_command<command_type::push_constant_command>(stage, offset, bytes, data);
    }

    void compute_command_buffer::disptach(vec3u const group_count) {
        //TODO
        EMBER_CHECK(false);
    }
}