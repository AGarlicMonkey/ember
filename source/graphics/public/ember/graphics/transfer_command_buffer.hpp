#pragma once

#include "ember/graphics/command_buffer.hpp"

#include <ember/core/export.hpp>
#include <ember/maths/vector.hpp>

namespace ember::graphics {
    class buffer;
    class image;
}

namespace ember::graphics {
    struct buffer_memory_barrier_info {
        //TODO
    };

    struct image_memory_barrier_info {
        //TODO
    };

    enum class pipeline_stage {
        //External Stages
        top, /**< Before any following stages commence. */

        //Transfer stages
        transfer, /**< All copy commands. */

        //Compute stages
        compute_shader, /**< Execution of a compute shader. */

        //Graphics stages
        vertex_input,             /**< Where vertex and index buffers are consumed. */
        vertex_shader,            /**< Execution of a vertex shader. */
        pixel_shader,             /**< Execution of a pixel shader. */
        early_pixel_test,         /**< Early depth and stencil tests. */
        late_pixel_test,          /**< Late depth and stencil tests. */
        colour_attachment_output, /**< After blending where the final colour values are output. This also includes depth attachments */
    };
}

namespace ember::graphics {
    class EMBER_API transfer_command_buffer : public command_buffer {
        //FUNCTIONS
    public:
        inline transfer_command_buffer();

        transfer_command_buffer(transfer_command_buffer const &other) = delete;
        inline transfer_command_buffer(transfer_command_buffer &&other) noexcept;

        transfer_command_buffer &operator=(transfer_command_buffer const &other) = delete;
        inline transfer_command_buffer &operator=(transfer_command_buffer &&other) noexcept;

        inline ~transfer_command_buffer() override;

        /**
         * @brief Copy the contents from one buffer to another.
         * @param source Buffer to copy from.
         * @param source_offset Offset into the source to start the copy.
         * @param destination Buffer to copy to.
         * @param destination_offset Offset into the destination to copy into.
         */
        void copy_buffer_to_buffer(buffer &source, std::size_t const source_offset, buffer &destination, std::size_t const destination_offset);
        /**
         * @brief Copy the contents from a buffer into an image.
         * @param source Buffer to copy from.
         * @param source_offset Offset into the source to start the copy.
         * @param destination Image to copy into.
         * @param destination_offset Offset into the destination to copy into.
         * @param destination_extent Extent of the copy into the destination.
         * @param destination_base_layer If the image is an array and/or a cube map, which index to start the copy from. Otherwise will be 0.
         * @param destination_layer_count If the image is an array and/or a cube map, how many layers to copy. Otherwise will be 1.
         */
        void copy_buffer_to_image(buffer &source, std::size_t const source_offset, image &destination, std::size_t const destination_offset, maths::vec3u const &destination_extent, std::uint32_t const destination_base_layer = 0, std::uint32_t const destination_layer_count = 1);

        /**
         * @brief Copy the contents of an image into a buffer.
         * @param source Image to copy from.
         * @param source_offset Offset into the source to copy from.
         * @param source_extent Exent of the copy from source.
         * @param source_base_layer If the image is an array and/or a cube map, which index to start the copy from. Otherwise will be 0.
         * @param source_layer_count If the image is an array and/or a cube map, how many layers to copy. Otherwise will be 1.
         * @param destination Buffer to copy to.
         * @param destination_offset  Offset into the destination to copy into.
         */
        void copy_image_to_buffer(image &source, maths::vec3i const &source_offset, maths::vec3u const source_extent, std::uint32_t const source_base_layer, std::uint32_t const source_layer_count, buffer &destination, std::size_t const destination_offset);

        /**
         * @brief Creates a memory barrier for a buffer. Controlling the execution order of commands on the buffer.
         * @details Any commands done on the buffer before the barrier are guarenteed to happen before commands
         * executed after the barrier. This barrier can also handle command queue transition of the buffer.
         * @param buffer The buffer to create the barrier for. 
         * @param barrier_info The information about the barrier.
         * @param source_tage The pipeline stage that gets executed before the barrier.
         * @param destination_stage The pipeline stage executed after the barrier that waits for the results of the sourceStage.
         */
        void buffer_memory_barrier(buffer &buffer, buffer_memory_barrier_info const &barrier_info, pipeline_stage const source_tage, pipeline_stage const destination_stage);
        /**
         * @brief Creates a memory barrier for a image. Controlling the execution order of commands on the image. 
         * @details Any commands done on the image before the barrier are guarenteed to happen before commands
         * executed after the barrier. This barrier can also handle command queue and image layout transition.
         * @param image The image to create the barrier for.
         * @param barrier_info The information about the barrier. 
         * @param source_tage The pipeline stage that gets executed before the barrier. 
         * @param destination_stage The pipeline stage executed after the barrier that waits for the results of the sourceStage.
         */
        void image_memory_barrier(image &image, image_memory_barrier_info const &barrier_info, pipeline_stage const source_tage, pipeline_stage const destination_stage);
    };
}

#include "transfer_command_buffer.inl"