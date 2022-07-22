#include "ember/graphics/graphics_command_buffer.hpp"

#include "commands.hpp"

namespace ember::inline graphics {
    void graphics_command_buffer::begin_render_pass(render_pass const *const render_pass, framebuffer const *const framebuffer, render_area const render_area, array<clear_value> clear_values) {
        record_command<command_type::begin_render_pass_command>(render_pass, framebuffer, render_area, std::move(clear_values));
    }

    void graphics_command_buffer::end_render_pass() {
        record_command<command_type::end_render_pass_command>();
    }

    void graphics_command_buffer::set_viewport(vec2i const position, vec2u const size, float const min_depth, float const max_depth) {
        record_command<command_type::set_viewport_command>(position, size, min_depth, max_depth);
    }

    void graphics_command_buffer::set_scissor(vec2i const position, vec2u const size) {
        record_command<command_type::set_scissor_command>(position, size);
    }

    void graphics_command_buffer::bind_pipeline_object(graphics_pipeline_object const *const pipeline_object) {
        record_command<command_type::bind_graphics_pipeline_object_command>(pipeline_object);
    }

    void graphics_command_buffer::bind_vertex_buffer(buffer const *const vertex_buffer, std::size_t const offset) {
        record_command<command_type::bind_vertex_buffer_command>(vertex_buffer, offset);
    }

    void graphics_command_buffer::bind_index_buffer(buffer const *const index_buffer, std::size_t const offset, index_type index_type) {
        record_command<command_type::bind_index_buffer_command>(index_buffer, offset, index_type);
    }

    void graphics_command_buffer::draw_indexed(std::size_t const index_count, std::uint32_t const instance_count, std::size_t const first_index, std::size_t const vertex_offset, std::uint32_t const first_instance) {
        record_command<command_type::draw_indexed_command>(index_count, instance_count, first_index, vertex_offset, first_instance);
    }

    void graphics_command_buffer::draw_indexed_indirect(buffer const *const indirect_buffer, std::size_t const offset, std::size_t const draw_count, std::size_t const stride) {
        record_command<command_type::draw_indexed_indirect_command>(indirect_buffer, offset, draw_count, stride);
    }
}