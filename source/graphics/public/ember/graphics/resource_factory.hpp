#pragma once

#include "ember/graphics/buffer.hpp"
#include "ember/graphics/descriptor_set_layout.hpp"
#include "ember/graphics/graphics_pipeline_object.hpp"
#include "ember/graphics/image.hpp"
#include "ember/graphics/image_view.hpp"
#include "ember/graphics/render_pass.hpp"

#include <ember/memory/unique_ptr.hpp>
#include <string_view>

namespace ember::graphics {
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
        virtual memory::unique_ptr<buffer> create_buffer(buffer::descriptor const &descriptor, std::string_view name) const = 0;
        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual memory::unique_ptr<image> create_image(image::descriptor descriptor, std::string_view name) const = 0;
        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual memory::unique_ptr<image_view> create_image_view(image const &image, image_view::descriptor const &descriptor, std::string_view name) const = 0;

        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual memory::unique_ptr<graphics_pipeline_object> create_graphics_pipeline_object(graphics_pipeline_object::descriptor const &descriptor, std::string_view name) const = 0;

        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual memory::unique_ptr<descriptor_set_layout> create_descriptor_set_layout(descriptor_set_layout::descriptor const &descriptor, std::string_view name) const = 0;

        /**
         * @brief
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual memory::unique_ptr<render_pass> create_render_pass(render_pass::descriptor const &descriptor, std::string_view name) const = 0;
    };
}