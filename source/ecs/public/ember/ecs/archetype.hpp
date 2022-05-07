#pragma once

#include "ember/ecs/types.hpp"

#include <ember/containers/array.hpp>
#include <ember/containers/map.hpp>
#include <ember/core/export.hpp>

namespace ember::ecs {
    /**
     * @brief Contains data for an entity archetype.
     * @details An entity archetype is defined by it's unique combination of components.
     */
    class EMBER_API archetype {
        //VARIABLES
    private:
        archetype_id_t id{};

        containers::map<entity, std::size_t> entities{}; /**< All entities belonging to this archetype. */
        containers::array<std::byte> component_data{};   /**< Memory pool for the archetype components. */

        containers::array<std::size_t> component_sizes{}; /**< Size of each individual component for the archetype. */
        std::size_t stride{ 0 };                          /**< Total size of all components. */

        //FUNCTIONS
    public:
        archetype() = delete;
        inline archetype(archetype_id_t id, containers::array<std::size_t> component_sizes);

        //TODO: Ctors etc.

        inline archetype_id_t const &get_id() const;

        void add_entity(entity entity);

        inline bool contains_entity(entity entity) const;
        inline bool allows_component(component_id_t component_id) const;

        template<typename component_t, typename... construct_args_t>
        component_t &alloc_component(entity entity, construct_args_t &&...construct_args);

        template<typename component_t>
        component_t &get_component(entity entity);
    };
}

#include "archetype.inl"