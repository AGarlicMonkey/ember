#pragma once

#include "ember/graphics/resource.hpp"

#include <cinttypes>
#include <cstddef>
#include <ember/core/enum.hpp>

namespace ember::inline graphics {
    class buffer {
        //TYPES
    public:
        using usage_mode_type = std::uint8_t;
        enum class usage_mode : usage_mode_type {
            transfer_source      = 1 << 0, /**< To be used as a source in a transfer operation. */
            transfer_destination = 1 << 1, /**< To be used as a destination in  a transfer operation. */
            vertex_buffer        = 1 << 2, /**< To be used as a vertex buffer. */
            index_buffer         = 1 << 3, /**< To be used as an index buffer. */
            uniform_buffer       = 1 << 4, /**< Uploaded to shaders as a uniform buffer. Uniform buffers are ready only and the size needs to be known by the shader. */
            storage_buffer       = 1 << 5, /**< Uploaded to shaders as a storage buffer. Storage buffers are read/write and can be of dynamic size in the shader. */
            indirect_buffer      = 1 << 6, /**< Can be used to store indirect commands for compute or draw distpatches. */
        };

        using access_type = std::uint16_t;
        enum class access : access_type {
            none,

            //Read
            vertex_shader_read_uniform_buffer,
            pixel_shader_read_uniform_buffer,
            compute_shader_read_uniform_buffer,
            transfer_read,

            //Write
            compute_shader_write_storage_buffer,
            transfer_write,
        };

        struct descriptor {
            std::size_t bytes{ 0 };
            usage_mode usage_flags{};
            sharing_mode sharing_mode{ sharing_mode::exclusive };
            memory_type memory_type{ memory_type::device_local };
        };

        //FUNCTIONS
    public:
        virtual ~buffer() = default;

        virtual descriptor const &get_descriptor() const = 0;

        /**
         * @brief Maps the region of a buffer into host accessible memory.
         * @param offset 
         * @param bytes 
         * @return 
         */
        virtual void *map(std::size_t const offset, std::size_t const bytes) = 0;

        /**
         * @brief Unmaps any previously mapped regions.
         */
        virtual void unmap() = 0;
    };

    EMBER_ENUM_BIT_FLAG_OPERATORS(buffer::usage_mode, buffer::usage_mode_type)
}