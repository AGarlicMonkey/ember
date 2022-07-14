#pragma once

#include "pipeline_stage.hpp"

#include <ember/containers/array.hpp>
#include <utility>

namespace ember::inline graphics {
    class semaphore;
    class graphics_command_buffer;
    class compute_command_buffer;
    class transfer_command_buffer;
}

namespace ember::inline graphics {
    template<typename command_buffer_t>
    struct submit_info {
        array<std::pair<semaphore const *, pipeline_stage>> wait_semaphores{}; /**< What semaphore each submission will wait on at which stage. */
        array<command_buffer_t const *> command_buffers{};                     /**< Which command buffers to execute. */
        array<semaphore const *> signal_semaphores{};                          /**< The semaphores that will be signaled when execution has finished.*/
    };

    using graphics_submit_info = submit_info<graphics_command_buffer>;
    using compute_submit_info  = submit_info<compute_command_buffer>;
    using transfer_submit_info = submit_info<transfer_command_buffer>;
}