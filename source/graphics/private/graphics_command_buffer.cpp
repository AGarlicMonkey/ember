#include "ember/graphics/graphics_command_buffer.hpp"

using namespace ember::maths;

namespace ember::graphics {
    void graphics_command_buffer::begin_render_pass(render_pass &render_pass, framebuffer &framebuffer, render_area const &render_area, std::span<clear_value> clear_values) {
        //TODO
    }

    void graphics_command_buffer::end_render_pass() {
        //TODO
    }

    void graphics_command_buffer::set_viewport(vec2i const position, vec2u const size) {
        //TODO
    }

    void graphics_command_buffer::set_scissor(vec2i const position, vec2u const size) {
        //TODO
    }

    void graphics_command_buffer::bind_pipeline_object(graphics_pipeline_object &pipeline_object) {
        //TODO
    }

    void graphics_command_buffer::bind_vertex_buffer(buffer &vertex_buffer, std::size_t const offset) {
        //TODO
    }

    void graphics_command_buffer::bind_index_buffer(buffer &index_buffer, std::size_t const offset, index_type index_type) {
        //TODO
    }

    void graphics_command_buffer::draw_indexed(std::size_t const index_count) {
        //TODO
    }
}