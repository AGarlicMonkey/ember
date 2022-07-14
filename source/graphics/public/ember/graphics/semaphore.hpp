#pragma once

namespace ember::inline graphics {
    /**
     * @brief A synchronisation object used to synchornise queues between each other.
     */
    class semaphore {
        //FUNCTIONS
    public:
        virtual ~semaphore() = default;
    };
}