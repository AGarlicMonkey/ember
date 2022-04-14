#pragma once

#include "ember/graphics/transfer_command_buffer.hpp"

#include <ember/core/export.hpp>
#include <ember/maths/vector.hpp>

namespace ember::graphics {
    class compute_pipeline_object;
    class descriptor_set;
}

namespace ember::graphics {
    class EMBER_API compute_command_buffer : public transfer_command_buffer {
        //FUNCTIONS
    public:
        inline compute_command_buffer();

        compute_command_buffer(compute_command_buffer const &other) = delete;
        inline compute_command_buffer(compute_command_buffer &&other) noexcept;

        compute_command_buffer &operator=(compute_command_buffer const &other) = delete;
        inline compute_command_buffer &operator=(compute_command_buffer &&other) noexcept;

        inline ~compute_command_buffer() override;

        /**
         * @brief 
         * @param pipeline_object 
         */
        void bind_pipeline_object(compute_pipeline_object &pipeline_object);

        /**
         * @brief 
         * @param descriptor_set 
         * @param set_num 
         */
        void bind_descriptor_set(descriptor_set &descriptor_set, std::uint32_t const set_num);
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
        void disptach(maths::vec3u group_count);
    };
}

#include "compute_command_buffer.inl"