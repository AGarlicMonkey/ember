#pragma once

namespace ember::inline graphics {
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