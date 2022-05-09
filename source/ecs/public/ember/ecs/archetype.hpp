#pragma once

#include "ember/ecs/component_helper.hpp"
#include "ember/ecs/types.hpp"

#include <ember/containers/array.hpp>
#include <ember/containers/map.hpp>
#include <ember/core/export.hpp>
#include <ember/memory/unique_ptr.hpp>

namespace ember::ecs {
    /**
     * @brief Contains data for an entity archetype.
     * @details An entity archetype is defined by it's unique combination of components.
     */
    class EMBER_API archetype {
        //TYPES
    private:
        /**
         * @brief Memory arena for the components in this archetype.
         * All components are stored contigously.
         */
        struct component_arena {
            std::byte *memory{ nullptr };
            std::size_t bytes{ 0 }; /**< Total size of the pool. */

            std::size_t stride{ 0 }; /**< Total size of all component types. */
            std::size_t allocations{ 0 };
        };

        //VARIABLES
    private:
        archetype_id_t id{};

        containers::map<entity, std::size_t> entities{};                                                        /**< All entities belonging to this archetype. */
        containers::map<component_id_t, memory::unique_ptr<internal::component_helpers>> *component_helper_map; /**< Maps a component id to it's helper type*/
        component_arena component_data{};                                                                       /**< Memory pool for the archetype's components. */

        //FUNCTIONS
    public:
        archetype() = delete;
        inline archetype(archetype_id_t id, containers::map<component_id_t, memory::unique_ptr<internal::component_helpers>> *component_helper_map);

        archetype(archetype const &other) = delete;
        inline archetype(archetype &&other) noexcept;

        archetype &operator=(archetype const &other) = delete;
        inline archetype &operator                   =(archetype &&other) noexcept;

        ~archetype();

        inline archetype_id_t const &get_id() const;

        void add_entity(entity const entity);
        void transfer_entity(entity const entity, archetype &previous_archetype);
        void remove_entity(entity const entity);

        inline bool contains_entity(entity const entity) const;
        inline bool allows_component(component_id_t const component_id) const;

        template<typename component_t, typename... construct_args_t>
        component_t &alloc_component(entity const entity, construct_args_t &&...construct_args);
        template<typename component_t>
        void destruct_component(entity const entity);

        template<typename component_t>
        component_t &get_component(entity const entity);

        template<typename function_t>
        void invoke_on_components(function_t function);

        template<typename function_t, typename object_t>
        void invoke_on_components(function_t function, object_t *object);

    private:
        void increase_arena_size();

        template<typename component_t>
        std::size_t get_component_offset() const;
        std::size_t get_component_offset(component_id_t const component_id) const;

        template<typename component_t>
        std::byte *get_component_memory(entity const entity) const;
        std::byte *get_component_memory(entity const entity, component_id_t const component_id) const;

        template<typename component_t>
        component_t & get_component_within_block(std::byte *const block);

        template<typename function_t, std::size_t... parameter_indices_t>
        void invoke(function_t function, std::index_sequence<parameter_indices_t...>);
        template<typename function_t, typename... components_t>
        void invoke(function_t function);

        template<typename function_t, typename object_t, std::size_t... parameter_indices_t>
        void invoke_member(function_t function, object_t *object, std::index_sequence<parameter_indices_t...>);
        template<typename function_t, typename object_t, typename... components_t>
        void invoke_member(function_t function, object_t *object);
    };
}

#include "archetype.inl"