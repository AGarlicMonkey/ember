#pragma once

#include "ember/ecs/archetype.hpp"
#include "ember/ecs/types.hpp"

#include <ember/containers/array.hpp>
#include <ember/containers/map.hpp>

namespace ember::ecs {
    /**
     * @brief Manages all currently known entity archetypes.
     */
    class component_manager {
        //VARIABLES
    private:
        containers::array<archetype> archetypes{};
        containers::map<entity, std::size_t> entity_to_archetype{};//Maps an entity to an index to the archetypes array;

        //FUNCTIONS
    public:
        template<typename component_t, typename... construct_args_t>
        component_t &add_component(entity entity, construct_args_t &&...construct_args);

        template<typename component_t>
        bool has_component(entity entity);

    private:
        /* archetype &get_archetype(archetype_id_t const &id); */
    };
}

#include "component_manager.inl"