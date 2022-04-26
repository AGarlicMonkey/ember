#pragma once

#include "ember/graphics/command_types.hpp"
#include "ember/graphics/compute_command_buffer.hpp"
#include "ember/graphics/graphics_command_buffer.hpp"
#include "ember/graphics/render_pass.hpp"
#include "ember/graphics/transfer_command_buffer.hpp"

#include <cinttypes>
#include <cstddef>
#include <ember/containers/array.hpp>
#include <ember/maths/vector.hpp>
#include <span>
#include <string>

namespace ember::graphics {
#define EMBER_GRAPHICS_CREATE_COMMAND(type) \
    template<>                              \
    struct recorded_command<type> : public command_destructor<recorded_command<type>>

    //Transfer command buffer
    EMBER_GRAPHICS_CREATE_COMMAND(command_type::copy_buffer_to_buffer_command){};

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::copy_buffer_to_image_command){};

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::copy_image_to_buffer_command){};

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::buffer_memory_barrier_command){};

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::image_memory_barrier_command){};

    //Compute command buffer
#if EMBER_GRAPHICS_ENABLE_USER_MARKERS
    EMBER_GRAPHICS_CREATE_COMMAND(command_type::push_user_marker_command) {
        std::string name;
        maths::vec4f colour;

        recorded_command(std::string name, maths::vec4f colour)
            : name{ name }
            , colour{ colour } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::pop_user_marker_command){};
#endif

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::bind_compute_pipeline_object_command){};

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::bind_descriptor_set_command){};

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::push_constant_command){};

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::dispatch_command){};

    //Graphics command buffer
    EMBER_GRAPHICS_CREATE_COMMAND(command_type::begin_render_pass_command) {
        render_pass const *const render_pass;
        framebuffer const *const framebuffer;
        render_area const render_area;
        containers::array<clear_value> clear_values;

        recorded_command(graphics::render_pass const *const render_pass, graphics::framebuffer const *const framebuffer, graphics::render_area const render_area, containers::array<clear_value> clear_values)
            : render_pass{ render_pass }
            , framebuffer{ framebuffer }
            , render_area{ render_area }
            , clear_values{ std::move(clear_values) } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::end_render_pass_command){};

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::set_viewport_command){};

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::set_scissor_command){};

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::bind_graphics_pipeline_object_command) {
        graphics_pipeline_object const *const pipeline_object;

        recorded_command(graphics_pipeline_object const *const pipeline_object)
            : pipeline_object{ pipeline_object } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::bind_vertex_buffer_command) {
        buffer const *const vertex_buffer;
        std::size_t const offset;

        recorded_command(buffer const *const vertex_buffer, std::size_t const offset)
            : vertex_buffer{ vertex_buffer }
            , offset{ offset } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::bind_index_buffer_command) {
        buffer const *const index_buffer;
        std::size_t const offset;
        index_type const index_type;

        recorded_command(buffer const *const index_buffer, std::size_t const offset, graphics::index_type const index_type)
            : index_buffer{ index_buffer }
            , offset{ offset }
            , index_type{ index_type } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::draw_indexed_command) {
        std::size_t const index_count;

        recorded_command(std::size_t const index_count)
            : index_count{ index_count } {
        }
    };

#undef EMBER_GRAPHICS_CREATE_COMMAND
}

namespace ember::graphics {
    std::size_t get_size_of_command(command_type const type);
}