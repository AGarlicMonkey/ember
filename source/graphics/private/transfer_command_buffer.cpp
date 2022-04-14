#include "ember/graphics/transfer_command_buffer.hpp"

using namespace ember::maths;

namespace ember::graphics {
    void transfer_command_buffer::copy_buffer_to_buffer(buffer &source, std::size_t const source_offset, buffer &destination, std::size_t const destination_offset) {
        //TODO
    }

    void transfer_command_buffer::copy_buffer_to_image(buffer &source, std::size_t const source_offset, image &destination, std::size_t const destination_offset, vec3u const &destination_extent, std::uint32_t const destination_base_layer, std::uint32_t const destination_layer_count) {
        //TODO
    }

    void transfer_command_buffer::copy_image_to_buffer(image &source, vec3i const &source_offset, vec3u const source_extent, std::uint32_t const source_base_layer, std::uint32_t const source_layer_count, buffer &destination, std::size_t const destination_offset) {
        //TODO
    }

    void transfer_command_buffer::buffer_memory_barrier(buffer &buffer, buffer_memory_barrier_info const &barrier_info, pipeline_stage const source_tage, pipeline_stage const destination_stage) {
        //TODO
    }

    void transfer_command_buffer::image_memory_barrier(image &image, image_memory_barrier_info const &barrier_info, pipeline_stage const source_tage, pipeline_stage const destination_stage) {
        //TODO
    }
}