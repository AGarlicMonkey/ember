#pragma once

#include "ember/graphics/graphics_command_buffer.hpp"

#include <cinttypes>
#include <ember/containers/array.hpp>
#include <utility>

namespace ember::graphics {
    class fence;
    class semaphore;
}

namespace ember::graphics {
    struct graphics_submit_info {
        containers::array<std::pair<semaphore const *, pipeline_stage>> wait_Semaphores{}; /**< What semaphore each submission will wait on at which stage. */
        containers::array<graphics_command_buffer const *> command_buffers{};              /**< Which command buffers to execute. */
        containers::array<semaphore const *> signal_semaphores{};                          /**< The semaphores that will be signaled when execution has finished.*/
    };
}

namespace ember::graphics {
    /**
     * @brief Allows allocation and submission of graphics command buffers.
     */
    class graphics_queue {
        //VARIABLES
    private:
        std::uint32_t queue_id{ 0 };//TODO: Unique ID given to command buffers so they can validate the queue they've been submitted to

        //FUNCTIONS
    public:
        inline graphics_queue();

        graphics_queue(graphics_queue const &other) = delete;
        inline graphics_queue(graphics_queue &&other) noexcept;

        graphics_queue &operator=(graphics_queue const &other) = delete;
        inline graphics_queue &operator=(graphics_queue &&other) noexcept;

        inline ~graphics_queue();

        /**
         * @brief 
         * @return 
         */
        inline graphics_command_buffer allocate_command_buffer();

        /**
         * @brief Submit command buffers to be processed. 
         * @details All buffers in a single submission will start in order but will likely finish out of order.
         * Each call to this submit function will need to wait on previous submissions.
         * @param submit_info 
         * @param signal_fence An optional fence that will be signaled when the submission is complete.
         */
        virtual void submit(graphics_submit_info const &submit_info, fence *signal_fence) = 0;
    };
}

#include "graphics_queue.inl"