#pragma once

#include "ember/graphics/buffer.hpp"
#include "ember/graphics/command_buffer.hpp"
#include "ember/graphics/image.hpp"
#include "ember/graphics/pipeline_stage.hpp"

#include <ember/core/export.hpp>
#include <ember/maths/vector.hpp>
#include <optional>

namespace ember::inline graphics {
    class buffer;
    class image;
}

namespace ember::inline graphics {
    enum class queue_type {
        none,
        graphics,
        compute,
        transfer,
    };

    struct buffer_memory_barrier_info {
        buffer::access previous_access;                   /**< How the buffer is being accessed before this barrier. */
        buffer::access next_access;                       /**< How the buffer is being accessed after this barrier. */
        queue_type source_queue{ queue_type::none };      /**< If transfering queue ownership, which queue to transfer from.*/
        queue_type destination_queue{ queue_type::none }; /**< If transfering queue ownership, which queue to transfer to.*/
    };

    struct image_memory_barrier_info {
        image::access previous_access;                    /**< How the image is being accessed before this barrier. */
        image::access next_access;                        /**< How the image is being accessed after this barrier. */
        queue_type source_queue{ queue_type::none };      /**< If transfering queue ownership, which queue to transfer from. */
        queue_type destination_queue{ queue_type::none }; /**< If transfering queue ownership, which queue to transfer to. */
        std::uint32_t base_layer{ 0 };                    /**< The lower bound image array layer to include in the barrier. */
        std::uint32_t layer_count{ 1 };                   /**< How many layers from the base layer to include in the barrier. */
    };
}

namespace ember::graphics {
    /**
     * @brief Can record transfer operations.
     */
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
         * @param bytes How many bytes to copy over
         */
        void copy_buffer_to_buffer(buffer const *const source, std::size_t const source_offset, buffer const *const destination, std::size_t const destination_offset, std::size_t const bytes);
        /**
         * @brief Copy the contents from a buffer into an image.
         * @details The size of the copy from the buffer is determined from the destination_extent param.
         * @param source Buffer to copy from.
         * @param source_offset Offset into the source to start the copy.
         * @param destination Image to copy into.
         * @param destination_offset Offset into the destination to copy into.
         * @param destination_extent Extent of the copy into the destination.
         * @param destination_base_layer If the image is an array and/or a cube map, which index to start the copy from. Otherwise will be 0.
         * @param destination_layer_count If the image is an array and/or a cube map, how many layers to copy. Otherwise will be 1.
         */
        void copy_buffer_to_image(buffer const *const source, std::size_t const source_offset, image const *const destination, vec3u const destination_offset, vec3u const destination_extent, std::uint32_t const destination_base_layer = 0, std::uint32_t const destination_layer_count = 1);

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
        void copy_image_to_buffer(image const *const source, vec3i const source_offset, vec3u const source_extent, std::uint32_t const source_base_layer, std::uint32_t const source_layer_count, buffer const *const destination, std::size_t const destination_offset);

        /**
         * @brief Creates an execution barrier. Controlling the execution order of commands between pipeline stages.
         * @details Makes sure that any commands executed before the barrier are completed before commands executed after
         * the barrier.
         * @param source_stage The pipeline stage that gets executed before the barrier.
         * @param destination_stage The pipeline stage executed after the barrier that waits for the results of the sourceStage.
         */
        void execution_barrier(pipeline_stage const source_stage, pipeline_stage const destination_stage);
        /**
         * @brief Creates a memory barrier for a buffer. Controlling the execution order of commands on the buffer.
         * @details Any commands done on the buffer before the barrier are guarenteed to happen before commands
         * executed after the barrier. This barrier can also handle command queue transition of the buffer.
         * @param buffer The buffer to create the barrier for. 
         * @param barrier_info The information about the barrier.
         */
        void buffer_memory_barrier(buffer const *const buffer, buffer_memory_barrier_info const barrier_info);
        /**
         * @brief Creates a memory barrier for a image. Controlling the execution order of commands on the image. 
         * @details Any commands done on the image before the barrier are guarenteed to happen before commands
         * executed after the barrier. This barrier can also handle command queue and image layout transition.
         * @param image The image to create the barrier for.
         * @param barrier_info The information about the barrier. 
         */
        void image_memory_barrier(image const *const image, image_memory_barrier_info const barrier_info);
    };
}

#include "transfer_command_buffer.inl"