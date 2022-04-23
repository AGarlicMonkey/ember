#pragma once

#include "pipeline_stage.hpp"

#include <ember/containers/array.hpp>
#include <utility>

namespace ember::graphics {
    class semaphore;
    class graphics_command_buffer;
    class compute_command_buffer;
    class transfer_command_buffer;
}

namespace ember::graphics {
    struct graphics_submit_info {
        containers::array<std::pair<semaphore const *, pipeline_stage>> wait_semaphores{}; /**< What semaphore each submission will wait on at which stage. */
        containers::array<graphics_command_buffer const *> command_buffers{};              /**< Which command buffers to execute. */
        containers::array<semaphore const *> signal_semaphores{};                          /**< The semaphores that will be signaled when execution has finished.*/
    };

    struct compute_submit_info {
        containers::array<std::pair<semaphore const *, pipeline_stage>> wait_semaphores{}; /**< What semaphore each submission will wait on at which stage. */
        containers::array<compute_command_buffer const *> command_buffers{};               /**< Which command buffers to execute. */
        containers::array<semaphore const *> signal_semaphores{};                          /**< The semaphores that will be signaled when execution has finished.*/
    };

    struct transfer_submit_info {
        containers::array<std::pair<semaphore const *, pipeline_stage>> wait_semaphores{}; /**< What semaphore each submission will wait on at which stage. */
        containers::array<transfer_command_buffer const *> command_buffers{};              /**< Which command buffers to execute. */
        containers::array<semaphore const *> signal_semaphores{};                          /**< The semaphores that will be signaled when execution has finished.*/
    };
}