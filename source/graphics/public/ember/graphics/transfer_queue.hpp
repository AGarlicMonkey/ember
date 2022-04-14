#pragma once

#include "ember/graphics/transfer_command_buffer.hpp"

#include <cinttypes>
#include <ember/containers/array.hpp>
#include <utility>

namespace ember::graphics {
    class fence;
    class semaphore;
}

namespace ember::graphics {
    struct transfer_submit_info {
        containers::array<std::pair<semaphore const *, pipeline_stage>> wait_Semaphores{}; /**< What semaphore each submission will wait on at which stage. */
        containers::array<transfer_command_buffer const *> command_buffers{};              /**< Which command buffers to execute. */
        containers::array<semaphore const *> signal_semaphores{};                          /**< The semaphores that will be signaled when execution has finished.*/
    };
}

namespace ember::graphics {
    /**
     * @brief Allows allocation and submission of transfer command buffers.
     */
    class transfer_queue {
        //VARIABLES
    private:
        std::uint32_t queue_id{ 0 };//TODO: Unique ID given to command buffers so they can validate the queue they've been submitted to

        //FUNCTIONS
    public:
        inline transfer_queue();

        transfer_queue(transfer_queue const &other) = delete;
        inline transfer_queue(transfer_queue &&other) noexcept;

        transfer_queue &operator=(transfer_queue const &other) = delete;
        inline transfer_queue &operator=(transfer_queue &&other) noexcept;

        inline ~transfer_queue();

        /**
         * @brief 
         * @return 
         */
        inline transfer_command_buffer allocate_command_buffer();

        /**
         * @brief Submit command buffers to be processed. 
         * @details All buffers in a single submission will start in order but will likely finish out of order.
         * Each call to this submit function will need to wait on previous submissions.
         * @param submit_info 
         * @param signal_fence An optional fence that will be signaled when the submission is complete.
         */
        virtual void submit(transfer_submit_info const &submit_info, fence *signal_fence) = 0;
    };
}

#include "transfer_queue.inl"