#pragma once

namespace ember::graphics {
    /**
     * @brief Represents an instance of a graphics API. The instance can
     * be used to quiery for and retrieve graphics devices to use for rendering.
     */
    class instance {
        //FUNCTIONS
    public:
        virtual ~instance() = default;
    };
}