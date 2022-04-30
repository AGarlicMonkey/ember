#include "ember/graphics/transfer_command_buffer.hpp"

#include "commands.hpp"

using namespace ember::maths;

namespace ember::graphics {
    void transfer_command_buffer::copy_buffer_to_buffer(buffer const *const source, std::size_t const source_offset, buffer const *const destination, std::size_t const destination_offset, std::size_t const bytes) {
        record_command<command_type::copy_buffer_to_buffer_command>(source, source_offset, destination, destination_offset, bytes);
    }

    void transfer_command_buffer::copy_buffer_to_image(buffer const *const source, std::size_t const source_offset, image const *const destination, maths::vec3u const destination_offset, maths::vec3u const destination_extent, std::uint32_t const destination_base_layer, std::uint32_t const destination_layer_count) {
        record_command<command_type::copy_buffer_to_image_command>(source, source_offset, destination, destination_offset, destination_extent, destination_base_layer, destination_layer_count);
    }

    void transfer_command_buffer::copy_image_to_buffer(image const *const source, maths::vec3i const source_offset, maths::vec3u const source_extent, std::uint32_t const source_base_layer, std::uint32_t const source_layer_count, buffer const *const destination, std::size_t const destination_offset) {
        //TODO
    }

    void transfer_command_buffer::buffer_memory_barrier(buffer const *const buffer, buffer_memory_barrier_info const barrier_info, pipeline_stage const source_tage, pipeline_stage const destination_stage) {
        //TODO
    }

    void transfer_command_buffer::image_memory_barrier(image const *const image, image_memory_barrier_info const barrier_info, pipeline_stage const source_tage, pipeline_stage const destination_stage) {
        //TODO
    }
}