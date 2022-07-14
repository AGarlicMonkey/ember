#include "memory_barrier_simplification.hpp"

namespace {
    ember::buffer_access_info const buffer_access_map[] = { //NOLINT TODO: Convert to containers::static_array when it's not an alias so it gets automatic type deduction
        { VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0 },//none

        { VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, VK_ACCESS_UNIFORM_READ_BIT },  //vertex_shader_read_uniform_buffer
        { VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_UNIFORM_READ_BIT },//pixel_shader_read_uniform_buffer
        { VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_ACCESS_UNIFORM_READ_BIT }, //compute_shader_read_uniform_buffer
        { VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_READ_BIT },      //transfer_read

        { VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_ACCESS_SHADER_WRITE_BIT },//compute_shader_write_storage_buffer
        { VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT },    //transfer_write
    };

    ember::image_access_info const image_access_map[] = { //NOLINT TODO: Convert to containers::static_array when it's not an alias so it gets automatic type deduction
        { VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, VK_IMAGE_LAYOUT_UNDEFINED },//none

        { VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL },                                                                          //pixel_shader_read_sampled
        { VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL },//depth_stencil_attachment_read
        { VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL },                                                                           //compute_shader_read_sampled
        { VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL },                                                                                   //transfer_read
        { VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR },                                                                                                            //present

        { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL },                                                         //colour_attachment_write
        { VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL },//depth_stencil_attachment_write
        { VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_ACCESS_SHADER_WRITE_BIT, VK_IMAGE_LAYOUT_GENERAL },                                                                                             //compute_shader_write_storage
        { VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL },                                                                                    //transfer_write
    };
}

namespace ember::inline graphics {
    buffer_access_info get_buffer_access(buffer::access const access) {
        return buffer_access_map[static_cast<buffer::access_type>(access)];
    }

    image_access_info get_image_access(image::access const access) {
        return image_access_map[static_cast<image::access_type>(access)];
    }
}