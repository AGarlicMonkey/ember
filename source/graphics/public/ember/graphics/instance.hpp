#pragma once

#include <ember/containers/array.hpp>

namespace ember::inline graphics {
    class device;
}

namespace ember::inline graphics {
    /**
     * @brief Represents an instance of a graphics API. The instance can
     * be used to quiery for and retrieve graphics devices to use for rendering.
     */
    class instance {
        //FUNCTIONS
    public:
        virtual ~instance() = default;

        /**
         * @brief Returns the most appropriate graphics device for real time rendering.
         * @details The lifetime of the device is tied to this instance.
         * @return 
         */
        virtual device *get_device() const noexcept = 0;
    };
}