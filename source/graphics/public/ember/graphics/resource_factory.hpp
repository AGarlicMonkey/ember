#pragma once

#include "ember/graphics/buffer.hpp"
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
         * @brief Create a render pass object
         * @param descriptor 
         * @param name 
         * @return 
         */
        virtual memory::unique_ptr<render_pass> create_render_pass(render_pass::descriptor const &descriptor, std::string_view name) const = 0;
    };
}