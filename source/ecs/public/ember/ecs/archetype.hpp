#pragma once

#include "ember/ecs/types.hpp"

#include <ember/containers/array.hpp>

namespace ember::ecs {
    /**
     * @brief Contains data for an entity archetype.
     * @details An entity archetype is defined by it's uniuqe combination of components.
     */
    struct archetype {
        archetype_id_t id{};
        containers::array<entity> entities{};                 /**< All entities belonging to this archetype. */
        //TODO needs to contain all components for all entities
        containers::array<std::byte> component_data{};        /**< Memory pool for the archetype components. */
        containers::array<std::size_t> component_data_size{}; /**< Size of each individual component for the archetype. */
    };

    //TODO: Perhaps make this a class to handle adding new entities??
}