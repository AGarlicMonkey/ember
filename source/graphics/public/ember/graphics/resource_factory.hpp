#pragma once

#include "ember/graphics/buffer.hpp"
#include "ember/graphics/compute_pipeline_object.hpp"
#include "ember/graphics/descriptor_pool.hpp"
#include "ember/graphics/descriptor_set_layout.hpp"
#include "ember/graphics/fence.hpp"
#include "ember/graphics/framebuffer.hpp"
#include "ember/graphics/graphics_pipeline_object.hpp"
#include "ember/graphics/image.hpp"
#include "ember/graphics/image_view.hpp"
#include "ember/graphics/render_pass.hpp"
#include "ember/graphics/sampler.hpp"
#include "ember/graphics/semaphore.hpp"

#include <ember/memory/unique_ptr.hpp>
#include <string_view>

namespace ember::inline graphics {
    /**
     * @brief 
     */
    class resource_factory {
        //FUNCTIONS
    public:
        virtual ~resource_factory() = default;

        /**
         * @brief 
         * @param descriptor 
         * @param name
         * @return 
         */
        virtual unique_ptr<buffer> create_buffer(buffer::descriptor descriptor, std::string_view name) const = 0;
        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual unique_ptr<image> create_image(image::descriptor descriptor, std::string_view name) const = 0;
        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual unique_ptr<image_view> create_image_view(image_view::descriptor descriptor, image const &image, std::string_view name) const = 0;
        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual unique_ptr<sampler> create_sampler(sampler::descriptor descriptor, std::string_view name) const = 0;

        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual unique_ptr<graphics_pipeline_object> create_graphics_pipeline_object(graphics_pipeline_object::descriptor descriptor, std::string_view name) const = 0;
        /**
         * @brief 
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual unique_ptr<compute_pipeline_object> create_compute_pipeline_object(compute_pipeline_object::descriptor descriptor, std::string_view name) const = 0;

        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual unique_ptr<descriptor_set_layout> create_descriptor_set_layout(descriptor_set_layout::descriptor descriptor, std::string_view name) const = 0;
        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual unique_ptr<descriptor_pool> create_descriptor_pool(descriptor_pool::descriptor descriptor, std::string_view name) const = 0;

        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual unique_ptr<render_pass> create_render_pass(render_pass::descriptor descriptor, std::string_view name) const = 0;
        /**
         * @brief 
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual unique_ptr<framebuffer> create_framebuffer(framebuffer::descriptor descriptor, std::string_view name) const = 0;

        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual unique_ptr<fence> create_fence(fence::descriptor descriptor, std::string_view name) const = 0;
        /**
         * @brief
         * @param name 
         * @return 
         */
        virtual unique_ptr<semaphore> create_semaphore(std::string_view name) const = 0;
    };
}