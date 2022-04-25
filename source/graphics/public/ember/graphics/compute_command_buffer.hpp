#pragma once

#include "ember/graphics/transfer_command_buffer.hpp"

#include <ember/core/export.hpp>
#include <ember/core/profiling.hpp>
#include <ember/maths/vector.hpp>
#include <string>

namespace ember::graphics {
    class compute_pipeline_object;
    class descriptor_set;
}

namespace ember::graphics {
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
        void push_user_marker(std::string name, maths::vec4f const colour);
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
         * @brief 
         * @param offset 
         * @param size 
         * @param data 
         */
        void push_constant(std::size_t const offset, std::size_t const size, void const *const data);

        /**
         * @brief Disptach a compute workload.
         * @param group_count How many groups on each axis to dispatch.
         */
        void disptach(maths::vec3u const group_count);
    };
}

#if EMBER_GRAPHICS_ENABLE_USER_MARKERS
namespace ember::graphics::internal {
    class scoped_user_marker {
    private:
        compute_command_buffer &buffer;

    public:
        scoped_user_marker(compute_command_buffer &buffer, std::string name, maths::vec4f const colour)
            : buffer{ buffer } {
            buffer.push_user_marker(std::move(name), colour);
        }

        //TODO: other ctors

        ~scoped_user_marker() {
            buffer.pop_user_marker();
        }
    };
}

    #define INTERNAL_EMBER_GRAPHICS_CAT2(a, b, c) a##b##c
    #define INTERNAL_EMBER_GRAPHICS_CAT(a, b, c) INTERNAL_EMBER_GRAPHICS_CAT2(a, b, c)

    /**
     * @brief Automatically pushses and pops a user marker.
     */
    #define EMBER_GRAPHICS_SCOPED_MARKER(command_buffer, name, colour)                                                                                      \
        ember::graphics::internal::scoped_user_marker INTERNAL_EMBER_GRAPHICS_CAT(scoped_user_marker, __COUNT__, __LINE__){ command_buffer, name, colour }; \
        EMBER_PROFILE_SCOPE(name)
#else
    #define EMBER_GRAPHICS_SCOPED_MARKER(command_buffer, name, colour)
#endif

#include "compute_command_buffer.inl"