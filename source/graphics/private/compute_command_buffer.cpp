#include "ember/graphics/compute_command_buffer.hpp"

using namespace ember::maths;

namespace ember::graphics {
    void compute_command_buffer::bind_pipeline_object(compute_pipeline_object &pipeline_object) {
        //TODO
    }

    void compute_command_buffer::bind_descriptor_set(descriptor_set &descriptor_set, std::uint32_t const set_num) {
        //TODO
    }

    void compute_command_buffer::push_constant(std::size_t const offset, std::size_t const size, void const *const data) {
        //TODO
    }

    void compute_command_buffer::disptach(vec3u group_count) {
        //TODO
    }
}