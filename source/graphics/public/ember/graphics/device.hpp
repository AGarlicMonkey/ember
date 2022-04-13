#pragma once

namespace ember::graphics {
    /**
     * @brief Represents a device that can recieve graphics operations. This device
     * could be a discrete GPU, an integrated GPU, a virtual GPU or a CPU.
     */
    class device {
        //FUNCTIONS
    public:
        virtual ~device() = default;
    };
}