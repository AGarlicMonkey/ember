#pragma once

#include "ember/graphics/compute_command_buffer.hpp"

#include <cinttypes>
#include <ember/containers/array.hpp>
#include <utility>

namespace ember::graphics {
    class fence;
    class semaphore;
}

namespace ember::graphics {
    struct compute_submit_info {
        containers::array<std::pair<semaphore const *, pipeline_stage>> wait_semaphores{}; /**< What semaphore each submission will wait on at which stage. */
        containers::array<compute_command_bufferconst *> command_buffers{};                /**< Which command buffers to execute. */
        containers::array<semaphore const *> signal_semaphores{};                          /**< The semaphores that will be signaled when execution has finished.*/
    };
}

namespace ember::graphics {
    /**
     * @brief Allows allocation and submission of compute command buffers.
     */
    class compute_queue {
        //VARIABLES
    private:
        std::uint32_t queue_id{ 0 };//TODO: Unique ID given to command buffers so they can validate the queue they've been submitted to

        //FUNCTIONS
    public:
        inline compute_queue();

        compute_queue(compute_queue const &other) = delete;
        inline compute_queue(compute_queue &&other) noexcept;

        compute_queue &operator=(compute_queue const &other) = delete;
        inline compute_queue &operator=(compute_queue &&other) noexcept;

        inline ~compute_queue();

        /**
         * @brief 
         * @return 
         */
        inline compute_command_buffer allocate_command_buffer();

        /**
         * @brief Submit command buffers to be processed. 
         * @details All buffers in a single submission will start and finish in order.
         * @param submit_info 
         * @param signal_fence An optional fence that will be signaled when the submission is complete.
         */
        virtual void submit(compute_submit_info const &submit_info, fence *signal_fence) = 0;
    };
}

#include "compute_queue.inl"