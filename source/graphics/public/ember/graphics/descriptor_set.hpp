#pragma once

#include "ember/graphics/descriptor.hpp"
#include "ember/graphics/image.hpp"

#include <cinttypes>
#include <cstddef>

namespace ember::graphics {
    class buffer;
    class image_view;
    class sampler;
}

namespace ember::graphics {
    /**
     * @brief Used to associate data into a binding inside a shader.
     * @details A 'set' is defined in the shader by: layout(set = 0, binding = 1). Whenever
     * this set is bound it will represent what ever number is passed to the 'bind_descriptor_set'
     * function on a command_buffer. This has to match the index of the descriptor_set_layout array
     * inside the appropriate pipeline object's descriptor. Each individual binding is written through
     * this descriptor set.
     * To update a descriptor set after it has been used in a draw or dispatch call it needs to be allocated
     * from a pool with the `descriptor_pool::flag::update_after_bind` flag set. Otherwise it can only be
     * written to the before it is bound for the first time.
     */
    class descriptor_set {
        //FUNCTIONS
    public:
        virtual ~descriptor_set() = default;

        /**
         * @brief Writes a region of a buffer into this descriptor set.
         * @param buffer The buffer to write from.
         * @param offset Offset into the buffer to start writing from.
         * @param bytes How many bytes from offset to write.
         * @param type The type of binding this is. In this case either uniform_buffer or storage_buffer.
         * @param slot Which slot inside the set this will be written to.
         */
        virtual void write(buffer const &buffer, std::size_t const offset, std::size_t const bytes, descriptor_type const type, std::uint32_t const slot) = 0;
        /**
         * @brief Writes an image into this descriptor set.
         * @param image_view A view of the image to write.
         * @param layout The expected layout of the image being written.
         * @param type The type of binding this is. In this case either sampled_image or storage_image.
         * @param slot Which slot inside the set this will be written to.
         */
        virtual void write(image_view const &image_view, image::layout const layout, descriptor_type const type, std::uint32_t const slot) = 0;
        /**
         * @brief Writes a sampler into a binding inside a shader.
         * @param sampler 
         * @param slot 
         */
        virtual void write(sampler const &sampler, std::uint32_t const slot) = 0;
    };
}