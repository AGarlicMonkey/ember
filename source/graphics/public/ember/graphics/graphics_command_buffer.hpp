#pragma once

#include "ember/graphics/compute_command_buffer.hpp"

#include <ember/containers/array.hpp>
#include <ember/core/export.hpp>
#include <ember/maths/vector.hpp>
#include <span>
#include <variant>

namespace ember::graphics {
    class render_pass;
    class framebuffer;
    class graphics_pipeline_object;
    class swapchain;
}

namespace ember::graphics {
    struct render_area {
        maths::vec2i origin{};
        maths::vec2u size{};
    };

    using colour_value = maths::vec4f;

    struct depth_stencil_value {
        float depth{ 0.0f };
        std::uint32_t stencil{ 0 };
    };

    using clear_value = std::variant<colour_value, depth_stencil_value>;

    enum class index_type {
        uint16
    };
}

namespace ember::graphics {
    /**
     * @brief Can record transfer, compute and graphics operations.
     */
    class EMBER_API graphics_command_buffer : public compute_command_buffer {
        //FUNCTIONS
    public:
        inline graphics_command_buffer();

        graphics_command_buffer(graphics_command_buffer const &other) = delete;
        inline graphics_command_buffer(graphics_command_buffer &&other) noexcept;

        graphics_command_buffer &operator=(graphics_command_buffer const &other) = delete;
        inline graphics_command_buffer &operator=(graphics_command_buffer &&other) noexcept;

        inline ~graphics_command_buffer() override;

        /**
         * @brief Begins a render pass. All subsiquent calls will use this render pass.
         * @param render_pass 
         * @param framebuffer 
         * @param render_area What area to render to in the framebuffer.
         * @param clear_values An a array of clear values. Each element in the array represents an attachment in the framebuffer.
         */
        void begin_render_pass(render_pass const *const render_pass, framebuffer const *const framebuffer, render_area const render_area, containers::array<clear_value> clear_values);
        /**
         * @brief Ends the current render pass. Must be called before starting a new one.
         */
        void end_render_pass();

        /**
         * @brief
         * @param position 
         * @param size 
         */
        void set_viewport(maths::vec2i const position, maths::vec2u const size, float const min_depth = 0.0f, float const max_depth = 1.0f);
        /**
         * @brief
         * @param position 
         * @param size 
         */
        void set_scissor(maths::vec2i const position, maths::vec2u const size);

        /**
         * @brief 
         * @param pipeline_object 
         */
        void bind_pipeline_object(graphics_pipeline_object const *const pipeline_object);

        /**
         * @brief Bind a vertex buffer to be used in the next draw call.
         * @param vertex_buffer 
         * @param offset Offset into the buffer where the vertices begin.
         */
        void bind_vertex_buffer(buffer const *const vertex_buffer, std::size_t const offset);
        /**
         * @brief Bind an index buffer to be used in the next draw call.
         * @param index_buffer 
         * @param offset Offset into the buffer where the indices begin.
         * @param index_type 
         */
        void bind_index_buffer(buffer const *const index_buffer, std::size_t const offset, index_type index_type);

        /**
         * @brief Executes a draw call with the currently bound vertex and index buffers.
         * @param index_count How many indices from the bound index buffer to draw.
         * @param instance_count How many instances to draw from this one draw call.
         * @param first_index First index in the index buffer to use.
         * @param vertex_offset Added to the vertex index before indexing into the vertex buffer.
         * @param first_instance The ID of the first instance of this draw call.
         */
        void draw_indexed(std::size_t const index_count, std::uint32_t const instance_count, std::size_t const first_index, std::size_t const vertex_offset, std::uint32_t const first_instance);

        /**
         * @brief Executes a draw indirect call with the currently bound vertex and index buffers.
         * @param indirect_buffer A buffer with one or many tightly packed draw_indexed_indirect_info written into it.
         * @param offset Offset into the buffer to look for the first draw_indexed_indirect_info.
         * @param draw_count How many draws to execute from the buffer.
         */
        void draw_indexed_indirect(buffer const *const indirect_buffer, std::size_t const offset, std::size_t const draw_count);
    };
}

#include "graphics_command_buffer.inl"