#pragma once

#include <cinttypes>
#include <ember/maths/vector.hpp>

#define EMBER_GRAPHICS_ENABLE_USER_MARKERS (EMBER_GRAPHICS_DEBUG_UTILITIES || EMBER_CORE_ENABLE_PROFILING)

namespace ember::inline graphics {
    enum class command_type : std::uint8_t {
        //Transfer
        copy_buffer_to_buffer_command,
        copy_buffer_to_image_command,
        copy_image_to_buffer_command,
        execution_barrier_command,
        buffer_memory_barrier_command,
        image_memory_barrier_command,

        //Compute
#if EMBER_GRAPHICS_ENABLE_USER_MARKERS
        push_user_marker_command,
        pop_user_marker_command,
#endif
        bind_compute_pipeline_object_command,
        bind_descriptor_set_command,
        push_constant_command,
        dispatch_command,
        dispatch_indirect_command,

        //Graphics
        begin_render_pass_command,
        end_render_pass_command,
        set_viewport_command,
        set_scissor_command,
        bind_graphics_pipeline_object_command,
        bind_vertex_buffer_command,
        bind_index_buffer_command,
        draw_indexed_command,
        draw_indexed_indirect_command,
    };

    //NOTE: The size of the buffer directly matches that of the vulkan indirect dispatch command
    struct dispatch_indirect_info {
        std::uint32_t x{};
        std::uint32_t y{};
        std::uint32_t z{};
    };

    //NOTE: The size of the buffer directly matches that of the vulkan indirect draw command
    struct draw_indexed_indirect_info {
        std::uint32_t index_count{};
        std::uint32_t instance_count{};
        std::uint32_t first_index{};
        std::int32_t vertex_offset{};
        std::uint32_t first_instance{};
    };

    struct command {
        command *next{ nullptr };
    };

    template<command_type type>
    struct recorded_command;
}