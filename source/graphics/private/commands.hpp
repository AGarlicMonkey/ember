#pragma once

#include "ember/graphics/command_types.hpp"
#include "ember/graphics/compute_command_buffer.hpp"
#include "ember/graphics/graphics_command_buffer.hpp"
#include "ember/graphics/render_pass.hpp"
#include "ember/graphics/shader.hpp"
#include "ember/graphics/transfer_command_buffer.hpp"

#include <cinttypes>
#include <cstddef>
#include <ember/containers/array.hpp>
#include <ember/maths/vector.hpp>
#include <ember/memory/memory.hpp>
#include <span>
#include <string>

namespace ember::inline graphics {
    class buffer;
}

namespace ember::inline graphics {
#define EMBER_GRAPHICS_CREATE_COMMAND(type) \
    template<>                              \
    struct recorded_command<type> : public command

    //Transfer command buffer
    EMBER_GRAPHICS_CREATE_COMMAND(command_type::copy_buffer_to_buffer_command) {
        buffer const *const source;
        std::size_t const source_offset;
        buffer const *const destination;
        std::size_t const destination_offset;
        std::size_t const bytes;

        recorded_command(buffer const *const source, std::size_t const source_offset, buffer const *const destination, std::size_t const destination_offset, std::size_t const bytes)
            : source{ source }
            , source_offset{ source_offset }
            , destination{ destination }
            , destination_offset{ destination_offset }
            , bytes{ bytes } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::copy_buffer_to_image_command) {
        buffer const *const source;
        std::size_t const source_offset;
        image const *const destination;
        vec3u const destination_offset;
        vec3u const destination_extent;
        std::uint32_t const destination_base_layer;
        std::uint32_t const destination_layer_count;

        recorded_command(buffer const *const source, std::size_t const source_offset, image const *const destination, vec3u const destination_offset, vec3u const destination_extent, std::uint32_t const destination_base_layer, std::uint32_t const destination_layer_count)
            : source{ source }
            , source_offset{ source_offset }
            , destination{ destination }
            , destination_offset{ destination_offset }
            , destination_extent{ destination_extent }
            , destination_base_layer{ destination_base_layer }
            , destination_layer_count{ destination_layer_count } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::copy_image_to_buffer_command) {
        image const *const source;
        vec3i const source_offset;
        vec3u const source_extent;
        std::uint32_t const source_base_layer;
        std::uint32_t const source_layer_count;
        buffer const *const destination;
        std::size_t const destination_offset;

        recorded_command(image const *const source, vec3i const source_offset, vec3u const source_extent, std::uint32_t const source_base_layer, std::uint32_t const source_layer_count, buffer const *const destination, std::size_t const destination_offset)
            : source{ source }
            , source_offset{ source_offset }
            , source_extent{ source_extent }
            , source_base_layer{ source_base_layer }
            , source_layer_count{ source_layer_count }
            , destination{ destination }
            , destination_offset{ destination_offset } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::execution_barrier_command) {
        pipeline_stage const source_stage;
        pipeline_stage const destination_stage;

        recorded_command(pipeline_stage const source_stage, pipeline_stage const destination_stage)
            : source_stage{ source_stage }
            , destination_stage{ destination_stage } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::buffer_memory_barrier_command) {
        buffer const *const buffer;
        buffer_memory_barrier_info const barrier_info;

        recorded_command(graphics::buffer const *const buffer, buffer_memory_barrier_info const barrier_info)
            : buffer{ buffer }
            , barrier_info{ barrier_info } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::image_memory_barrier_command) {
        image const *const image;
        image_memory_barrier_info const barrier_info;

        recorded_command(graphics::image const *const image, image_memory_barrier_info const barrier_info)
            : image{ image }
            , barrier_info{ barrier_info } {
        }
    };

        //Compute command buffer
#if EMBER_GRAPHICS_ENABLE_USER_MARKERS
    EMBER_GRAPHICS_CREATE_COMMAND(command_type::push_user_marker_command) {
        std::string name;
        vec4f colour;

        recorded_command(std::string name, vec4f colour)
            : name{ name }
            , colour{ colour } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::pop_user_marker_command){};
#endif

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::bind_compute_pipeline_object_command) {
        compute_pipeline_object const *const pipeline_object;

        recorded_command(compute_pipeline_object const *const pipeline_object)
            : pipeline_object{ pipeline_object } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::bind_descriptor_set_command) {
        descriptor_set const *const descriptor_set;
        std::uint32_t const set_num;

        recorded_command(graphics::descriptor_set const *const descriptor_set, std::uint32_t const set_num)
            : descriptor_set{ descriptor_set }
            , set_num{ set_num } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::push_constant_command) {
        shader::stage const stage;
        std::size_t const offset;
        std::size_t const bytes;
        std::byte *data;

        recorded_command(shader::stage const stage, std::size_t const offset, std::size_t const bytes, void const *const data)
            : stage{ stage }
            , offset{ offset }
            , bytes{ bytes } {
            //Take a copy of the data as we cannot guarentee the lifetime.
            this->data = memory::alloc(bytes, 0);
            std::memcpy(this->data, data, bytes);
        }
        ~recorded_command() {
            memory::free(data);
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::dispatch_command) {
        vec3u const group_count;

        recorded_command(vec3u const group_count)
            : group_count{ group_count } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::dispatch_indirect_command) {
        buffer const *const indirect_buffer;
        std::size_t const offset;

        recorded_command(buffer const *const indirect_buffer, std::size_t const offset)
            : indirect_buffer{ indirect_buffer }
            , offset{ offset } {
        }
    };

    //Graphics command buffer
    EMBER_GRAPHICS_CREATE_COMMAND(command_type::begin_render_pass_command) {
        render_pass const *const render_pass;
        framebuffer const *const framebuffer;
        render_area const render_area;
        array<clear_value> clear_values;

        recorded_command(graphics::render_pass const *const render_pass, graphics::framebuffer const *const framebuffer, graphics::render_area const render_area, array<clear_value> clear_values)
            : render_pass{ render_pass }
            , framebuffer{ framebuffer }
            , render_area{ render_area }
            , clear_values{ std::move(clear_values) } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::end_render_pass_command){};

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::set_viewport_command) {
        vec2i const position;
        vec2u const size;
        float const min_depth;
        float const max_depth;

        recorded_command(vec2i const position, vec2u const size, float const min_depth, float const max_depth)
            : position{ position }
            , size{ size }
            , min_depth{ min_depth }
            , max_depth{ max_depth } {}
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::set_scissor_command) {
        vec2i const position;
        vec2u const size;

        recorded_command(vec2i const position, vec2u const size)
            : position{ position }
            , size{ size } {
        }
    };

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
        std::uint32_t const instance_count;
        std::size_t const first_index;
        std::size_t const vertex_offset;
        std::uint32_t const first_instance;

        recorded_command(std::size_t const index_count, std::uint32_t const instance_count, std::size_t const first_index, std::size_t const vertex_offset, std::uint32_t const first_instance)
            : index_count{ index_count }
            , instance_count{ instance_count }
            , first_index{ first_index }
            , vertex_offset{ vertex_offset }
            , first_instance{ first_instance } {
        }
    };

    EMBER_GRAPHICS_CREATE_COMMAND(command_type::draw_indexed_indirect_command) {
        buffer const *const indirect_buffer;
        std::size_t const offset;
        std::size_t const draw_count;
        std::size_t const stride;

        recorded_command(buffer const *const indirect_buffer, std::size_t const offset, std::size_t const draw_count, std::size_t const stride)
            : indirect_buffer{ indirect_buffer }
            , offset{ offset }
            , draw_count{ draw_count }
            , stride{ stride } {
        }
    };

#undef EMBER_GRAPHICS_CREATE_COMMAND
}

namespace ember::inline graphics {
    std::size_t get_size_of_command(command_type const type);
    void destruct_command(command_type const type, std::byte *const command_memory);
}