#pragma once

#include "ember/graphics/shader.hpp"
#include "ember/graphics/transfer_command_buffer.hpp"

#include <ember/core/export.hpp>
#include <ember/core/profiling.hpp>
#include <ember/maths/vector.hpp>
#include <string>

namespace ember::inline graphics {
    class compute_pipeline_object;
    class descriptor_set;
}

namespace ember::inline graphics {
    /**
     * @brief Can record transfer and compute operations.
     */
    class EMBER_API compute_command_buffer : public transfer_command_buffer {
        //FUNCTIONS
    public:
        inline compute_command_buffer();

        compute_command_buffer(compute_command_buffer const &other) = delete;
        inline compute_command_buffer(compute_command_buffer &&other) noexcept;

        compute_command_buffer &operator=(compute_command_buffer const &other) = delete;
        inline compute_command_buffer &operator=(compute_command_buffer &&other) noexcept;

        inline ~compute_command_buffer() override;

#if EMBER_GRAPHICS_ENABLE_USER_MARKERS
        /**
         * @brief Pushes a user marker into the command buffer.
         * @param name 
         */
        void push_user_marker(std::string name, vec4f const colour);
        /**
         * @brief Pops the most recently pushed command from the buffer.
         */
        void pop_user_marker();
#endif

        /**
         * @brief 
         * @param pipeline_object 
         */
        void bind_pipeline_object(compute_pipeline_object const *const pipeline_object);

        /**
         * @brief 
         * @param descriptor_set 
         * @param set_num 
         */
        void bind_descriptor_set(descriptor_set const *const descriptor_set, std::uint32_t const set_num);
        /**
         * @brief Push constant data into the command buffer to be accessed by shaders.
         * @param stage Which shader stage to upload to.
         * @param offset The offset into the push constant range to upload to (Signified in the shader with layout(push_constant) {layout(offset = x) ... }).
         * @param bytes The size of the push constant range to update.
         * @param data The data to upload. Will copy bytes amount from the beginning of the pointer into the command buffer.
         */
        void push_constant(shader::stage const stage, std::size_t const offset, std::size_t const bytes, void const *const data);

        /**
         * @brief Disptach a compute workload.
         * @param group_count How many groups on each axis to dispatch.
         */
        void dispatch(vec3u const group_count);

        /**
         * @brief Dispatch an indirect compute workload.
         * @param indirect_buffer A buffer with one or many tightly packed dispatch_indirect_info written into it.
         * @param offset Offset into the indirect_buffer to start the dispatch from.
         */
        void dispatch_indirect(buffer const *const indirect_buffer, std::size_t const offset);
    };
}

#if EMBER_GRAPHICS_ENABLE_USER_MARKERS
namespace ember::inline graphics::internal {
    class scoped_user_marker {
    private:
        compute_command_buffer &buffer;

    public:
        scoped_user_marker() = delete;
        scoped_user_marker(compute_command_buffer &buffer, std::string name)
            : buffer{ buffer } {
            buffer.push_user_marker(std::move(name), vec4f{ 0, 0, 0, 0 });
        }
        scoped_user_marker(compute_command_buffer &buffer, std::string name, vec4f const colour)
            : buffer{ buffer } {
            buffer.push_user_marker(std::move(name), colour);
        }

        scoped_user_marker(scoped_user_marker const &other)     = delete;
        scoped_user_marker(scoped_user_marker &&other) noexcept = delete;

        scoped_user_marker &operator=(scoped_user_marker const &other)     = delete;
        scoped_user_marker &operator=(scoped_user_marker &&other) noexcept = delete;

        ~scoped_user_marker() {
            buffer.pop_user_marker();
        }
    };
}

    #define INTERNAL_EMBER_GRAPHICS_CAT2(a, b, c) a##b##c
    #define INTERNAL_EMBER_GRAPHICS_CAT(a, b, c) INTERNAL_EMBER_GRAPHICS_CAT2(a, b, c)

    #define EMBER_GRAPHICS_SCOPED_MARKER(command_buffer, name)                                                                                        \
        ::ember::graphics::internal::scoped_user_marker INTERNAL_EMBER_GRAPHICS_CAT(scoped_user_marker, __COUNT__, __LINE__){ command_buffer, name }; \
        EMBER_PROFILE_SCOPE(name)

    #define EMBER_GRAPHICS_SCOPED_MARKER_C(command_buffer, name, r, g, b)                                                                                                                     \
        ::ember::graphics::internal::scoped_user_marker INTERNAL_EMBER_GRAPHICS_CAT(scoped_user_marker, __COUNT__, __LINE__){ command_buffer, name, ::ember::maths::vec4f{ r, g, b, 1.0f } }; \
        EMBER_PROFILE_SCOPE(name)
#else
    #define EMBER_GRAPHICS_SCOPED_MARKER(command_buffer, name)
    #define EMBER_GRAPHICS_SCOPED_MARKER_C(command_buffer, name, r, g, b)
#endif

#include "compute_command_buffer.inl"