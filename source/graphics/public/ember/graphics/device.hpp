#pragma once

#include "ember/graphics/swapchain.hpp"

#include <ember/memory/unique_ptr.hpp>

namespace ember::platform {
    class window;
}

namespace ember::graphics {
    /**
     * @brief Represents a device that can recieve graphics operations. This device
     * could be a discrete GPU, an integrated GPU, a virtual GPU or a CPU.
     */
    class device {
        //FUNCTIONS
    public:
        virtual ~device() = default;
        /**
         * @brief Creates a swapchain for a given window.
         * @param descriptor 
         * @param window 
         * @return 
         */
        virtual memory::unique_ptr<swapchain> create_swapchain(swapchain::descriptor descriptor, platform::window const &window) const = 0;
    };
}