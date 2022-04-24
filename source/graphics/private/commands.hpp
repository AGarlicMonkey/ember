#pragma once

#include "ember/graphics/command_types.hpp"
#include "ember/graphics/compute_command_buffer.hpp"
#include "ember/graphics/graphics_command_buffer.hpp"
#include "ember/graphics/render_pass.hpp"
#include "ember/graphics/transfer_command_buffer.hpp"

#include <cinttypes>
#include <cstddef>
#include <span>
#include <string>
#include <ember/containers/array.hpp>
#include <ember/maths/vector.hpp>

namespace ember::graphics {
    //Transfer command buffer
    template<>
    struct recorded_command<command_type::copy_buffer_to_buffer_command> {
    };

    template<>
    struct recorded_command<command_type::copy_buffer_to_image_command> {
    };

    template<>
    struct recorded_command<command_type::copy_image_to_buffer_command> {
    };

    template<>
    struct recorded_command<command_type::buffer_memory_barrier_command> {
    };

    template<>
    struct recorded_command<command_type::image_memory_barrier_command> {
    };

    //Compute command buffer
#if EMBER_GRAPHICS_DEBUG_UTILITIES || EMBER_CORE_ENABLE_PROFILING
    template<>
    struct recorded_command<command_type::push_user_marker_command> {
        std::string name{};
        maths::vec4f colour{};
    };

    template<>
    struct recorded_command<command_type::pop_user_marker_command> {};
#endif

    template<>
    struct recorded_command<command_type::bind_compute_pipeline_object_command> {
    };

    template<>
    struct recorded_command<command_type::bind_descriptor_set_command> {
    };

    template<>
    struct recorded_command<command_type::push_constant_command> {
    };

    template<>
    struct recorded_command<command_type::dispatch_command> {
    };

    //Graphics command buffer
    template<>
    struct recorded_command<command_type::begin_render_pass_command> {
        render_pass const *const render_pass;
        framebuffer const *const framebuffer;
        render_area const render_area;
        containers::array<clear_value> clear_values;
    };

    template<>
    struct recorded_command<command_type::end_render_pass_command> {
    };

    template<>
    struct recorded_command<command_type::set_viewport_command> {
    };

    template<>
    struct recorded_command<command_type::set_scissor_command> {
    };

    template<>
    struct recorded_command<command_type::bind_graphics_pipeline_object_command> {
        graphics_pipeline_object const *const pipeline_object;
    };

    template<>
    struct recorded_command<command_type::bind_vertex_buffer_command> {
        buffer const *const vertex_buffer;
        std::size_t const offset;
    };

    template<>
    struct recorded_command<command_type::bind_index_buffer_command> {
        buffer const *const index_buffer;
        std::size_t const offset;
        index_type index_type;
    };

    template<>
    struct recorded_command<command_type::draw_indexed_command> {
        std::size_t const index_count;
    };
}

namespace ember::graphics {
    std::size_t get_size_of_command(command_type const type);
}