#pragma once

#include "ember/graphics/descriptor.hpp"

#include <cinttypes>
#include <ember/containers/array.hpp>
#include <ember/core/enum.hpp>
#include <ember/memory/unique_ptr.hpp>

namespace ember::inline graphics {
    class descriptor_set;
    class descriptor_set_layout;

    struct descriptor_type_info {
        descriptor_type type{};
        std::uint32_t count{}; /**< Number of this type to allocate across all potential descriptor sets allocated from the pool. */
    };
}

namespace ember::inline graphics {
    /**
     * @brief Used to allocate descriptor_sets. A descriptor_pool is not tied to a specific descriptor_set_layout.
     */
    class descriptor_pool {
        //TYPES
    public:
        using flag_type = std::uint8_t;
        enum class flag : flag_type {
            none              = 0 << 0,
            update_after_bind = 1 << 0, /**< Allows this pool to allocate descriptor sets that are able to be updated after bound for the first time. See `descriptor_set_binding_info::bindless`. */
        };

        struct descriptor {
            array<descriptor_type_info> descriptor_types{}; /**< What and how many types to pool for allocation. */
            std::uint32_t max_sets{ 1 };                    /**< The maximum amount of sets that can be allocated from this pool. */
            flag flags{ flag::none };
        };

        //FUNCTIONS
    public:
        virtual ~descriptor_pool() = default;

        virtual descriptor const &get_descriptor() const = 0;

        /**
         * @brief Allocates a single descriptor set from this pool.
         * @param layout 
         * @return 
         */
        virtual unique_ptr<descriptor_set> allocate_descriptor_set(descriptor_set_layout const &layout) = 0;
        /**
         * @brief Allocates multiple descriptor sets from this pool.
         * @param layouts 
         * @return 
         */
        virtual array<unique_ptr<descriptor_set>> allocate_descriptor_sets(array<descriptor_set_layout const *> const &layouts) = 0;

        /**
         * @brief Reset all allocated sets from this pool. 
         * @details This will only internally reset this pool. Any pointers to previously
         * allocated sets need to be deleted manually.
         */
        virtual void reset() = 0;
    };

    EMBER_ENUM_BIT_FLAG_OPERATORS(descriptor_pool::flag, descriptor_pool::flag_type);
}