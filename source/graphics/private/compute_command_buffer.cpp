#include "ember/graphics/compute_command_buffer.hpp"

#include "commands.hpp"

using namespace ember::maths;

namespace ember::graphics {
#if EMBER_GRAPHICS_DEBUG_UTILITIES
    void compute_command_buffer::push_user_marker(std::string name) {
        record_command<command_type::push_user_marker_command>(std::move(name));
    }

    void compute_command_buffer::pop_user_marker() {
        record_command<command_type::pop_user_marker_command>();
    }
#endif

    void compute_command_buffer::bind_pipeline_object(compute_pipeline_object const *const pipeline_object) {
        //TODO
    }

    void compute_command_buffer::bind_descriptor_set(descriptor_set const *const descriptor_set, std::uint32_t const set_num) {
        //TODO
    }

    void compute_command_buffer::push_constant(std::size_t const offset, std::size_t const size, void const *const data) {
        //TODO
    }

    void compute_command_buffer::disptach(vec3u const group_count) {
        //TODO
    }
}