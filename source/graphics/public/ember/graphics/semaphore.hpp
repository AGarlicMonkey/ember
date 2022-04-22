#pragma once

namespace ember::graphics {
    /**
     * @brief A synchronisation object used to synchornise queues between each other.
     */
    class semaphore {
        //FUNCTIONS
    public:
        virtual ~semaphore() = default;
    };
}