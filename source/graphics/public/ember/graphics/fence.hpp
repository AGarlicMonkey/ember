#pragma once

namespace ember::graphics {
    /**
     * @brief A synchronisation object used to synchronise graphics operations with the host application.
     */
    class fence {
        //TYPES
    public:
        struct descriptor {
            bool signaled{ false }; /**< if true, will initialise the fence as already being signaled. */
        };

        //FUNCTIONS
    public:
        virtual ~fence() = default;

        virtual descriptor const &get_descriptor() const = 0;

        /**
         * @brief Makes the current thread wait until this fence is signaled.
         */
        virtual void wait() = 0;
        /**
         * @brief Checks to see if the fence has been signaled or not.
         * @return True if signaled false if unsignaled.
         */
        virtual bool poll() = 0;
        
        /**
         * @brief Resets the fence back into an unsignaled state.
         */
        virtual void reset() = 0;
    };
}