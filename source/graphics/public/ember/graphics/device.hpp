#pragma once

#include "ember/graphics/submission_types.hpp"
#include "ember/graphics/swapchain.hpp"

#include <ember/memory/unique_ptr.hpp>

namespace ember::platform {
    class window;
}

namespace ember::graphics {
    class resource_factory;
    class shader_cache;
    class fence;
    class semaphore;

    class graphics_queue;
    class compute_queue;
    class transfer_queue;
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
         * @brief Returns the factory object used to create graphics resources.
         * @details Lifetime of the factory is tied to this object.
         * @return 
         */
        virtual resource_factory const *get_factory() const = 0;
        /**
         * @brief Returns the shader cache which contains the applications compiled shaders.
         * @details Lifetime of the cache is tied to this object.
         * @return 
         */
        virtual shader_cache *get_shader_cache() = 0;

        /**
         * @brief Creates a swapchain for a given window.
         * @param descriptor 
         * @param window 
         * @return 
         */
        virtual memory::unique_ptr<swapchain> create_swapchain(swapchain::descriptor descriptor, platform::window const &window) const = 0;

        /**
         * @brief 
         * @param submit_info 
         * @param signal_fence 
         */
        virtual void submit_to_graphics_queue(graphics_submit_info const &submit_info, fence const *const signal_fence) = 0;
        /**
         * @brief 
         * @param submit_info 
         * @param signal_fence 
         */
        virtual void submit_to_compute_queue(compute_submit_info const &submit_info, fence const *const signal_fence) = 0;
        /**
         * @brief 
         * @param submit_info 
         * @param signal_fence 
         */
        virtual void submit_to_transfer_queue(transfer_submit_info const &submit_info, fence const *const signal_fence) = 0;

        /**
         * @brief Presents the swapchain to the window it's attached to.
         * @param swapchain The swapchain to present.
         * @param image_index The index of the image in the swapchain to present.
         * @param wait_semaphore The semaphore to wait on before presenting. Usually signaled when the image is done being rendered to.
         */
        virtual swapchain::result present_swapchain(swapchain const *const swapchain, std::size_t const image_index, semaphore const *const wait_semaphore) = 0;

        /**
         * @brief Pauses the current thread until the device is idle.
         */
        virtual void wait_until_idle() const = 0;
    };
}